<?php

// $Id$

/**
 * @file
 *
 * Handles Ajax Requests.
 */
class AjaxHandler {

  /**
   * Object PID.
   * 
   * @var string
   */
  protected $pid;
  /**
   * 
   * @var Fedora_Item
   */
  protected $item;
  /**
   * Access through functions not directly as it may not exist.
   * 
   * @var string
   */
  protected $foxml;
  protected $xpath;

  /**
   *
   * @param string $pid
   */
  public function __construct($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->pid = $pid;
    $this->item = new Fedora_Item($pid);
  }

  /**
   *
   * @return string
   */
  public function getObjectProperties() {
    $object_properties = array(
      'label' => $this->getObjectLabel(),
      'created' => $this->getObjectCreationDate(),
      'modified' => $this->getObjectLastModifiedDate(),
      'owner' => $this->getObjectOwner(),
      'state' => $this->getObjectState()
    );
    return array($object_properties);
  }

  public function setObjectProperties($values) {
    $states = array(
      'Active' => 'A',
      'Inactive' => 'I',
      'Deleted' => 'D',
    );
    $label = $values['label'];
    $owner = $values['owner'];
    $state = $values['state'];
    if ($this->item->modify_object($label, $states[$state], $owner)) {
      $object_properties = $this->getObjectProperties();
      return array('success' => true, 'msg' => 'Successfuly Edited Object', 'data' => $object_properties);
    }
    else {
      $object_properties = $this->getObjectProperties();
      return array('success' => false, 'msg' => 'Failed to Edit Object', 'data' => $object_properties);
    }
  }

  private function getObjectLabel() {
    return $this->item->objectProfile->objLabel;
  }

  private function getObjectCreationDate() {
    $date_created = new DateTime($this->item->objectProfile->objCreateDate);
    return $date_created->format('F j, Y');
  }

  private function getObjectLastModifiedDate() {
    $date_modified = new DateTime($this->item->objectProfile->objLastModDate);
    return $date_modified->format('F j, Y');
  }

  private function getObjectOwner() {
    $xpath = $this->getFoxmlXPath();
    $result = $xpath->query("/foxml:digitalObject/foxml:objectProperties/foxml:property[@NAME='info:fedora/fedora-system:def/model#ownerId']/@VALUE");
    if ($result && $result->length == 1) {
      return $result->item(0)->value;
    }
    return '';
  }

  private function getObjectState() {
    $xpath = $this->getFoxmlXPath();
    $result = $xpath->query("/foxml:digitalObject/foxml:objectProperties/foxml:property[@NAME='info:fedora/fedora-system:def/model#state']/@VALUE");
    if ($result && $result->length == 1) {
      return $result->item(0)->value;
    }
    return '';
  }

  /**
   * Gets this objects FOXML repersentation, as a DOMDocument.
   *
   * Creates a new DOMDocument object from this objects exported FOXML, storing it
   * as a member if it doesn't already exist.
   *
   * @return DOMDocument
   */
  private function getFoxmlDocument() {
    if (empty($this->foxml)) {
      $foxml = $this->item->export_as_foxml();
      $this->foxml = new DOMDocument();
      $this->foxml->loadXML($foxml);
      return $this->foxml;
    }
    else {
      return $this->foxml;
    }
  }

  /**
   * Get the XPath for this object FOXML repersentation.
   *
   * Creates a new DOMXPath object, storing it as a member if it doesn't already exist.
   *
   * @return DOMXPath
   *   XPath for this object FOXML repersentation.
   */
  private function getFoxmlXPath() {
    if (empty($this->xpath)) {
      $foxml = $this->getFoxmlDocument();
      $this->xpath = new DOMXPath($foxml);
      $this->registerFoxmlNamespaces($foxml->saveXML());
      return $this->xpath;
    }
    else {
      return $this->xpath;
    }
  }

  /**
   * Registers all namespaces found in the foxml document.
   *
   * @param string $foxml
   *   XML string of the exported fedora objects foxml.
   */
  private function registerFoxmlNamespaces($foxml) {
    $simple = new SimpleXMLElement($foxml);
    $names = $simple->getNamespaces(TRUE);
    foreach ($names as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

}
