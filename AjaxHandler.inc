<?php

// $Id$

/**
 * @file
 *
 * Defines a class that handles Ajax Requests.
 */

/**
 * Handles Ajax requests.
 */
class AjaxHandler {

  /**
   * Fedora Object PID.
   * 
   * @var string
   */
  protected $pid;
  /**
   * Fedora Object
   *
   * @var Fedora_Item
   */
  protected $item;
  /**
   * FOXML repersention of the Fedora Object identified by $pid.
   *
   * Access through the function AjaxHandler::getFoxmlDocument().
   * Do not access directly as it may not exist.
   * 
   * @var DOMDocument
   */
  protected $foxml;
  /**
   * XPath for the FOXML document identified by $foxml.
   *
   * Access through the function AjaxHandler::getFoxmlXPath().
   * Do not directly as it may not exist.
   *
   * @var DOMDocument
   */
  protected $xpath;

  /**
   * Create a Ajax request handler.
   *
   * This class provides methods for accesing/modifiying infomation about the
   * fedora objected identified by $pid. These methods are general accessed by
   * Ajax requests, from the ADR Basic Viewer.
   *
   * @param string $pid
   *   Fedora Object PID.
   */
  public function __construct($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->pid = $pid;
    $this->item = new Fedora_Item($pid);
  }

  /**
   * Gets the fedora object's properties.
   *
   * @return array
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

  /**
   * Sets the fedora object's properties.
   *
   * Input is expected to be from the form defined on the manage tab,
   * of the ADR Basic viewer.
   *
   * @return array
   *   Indicates the success of this function as well as the fedora
   *   objects current settings.
   */
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

  /**
   * Deletes the fedora object in question.
   *
   * @return array
   */
  public function deleteObject() {
    if ($this->item->purge()) {
      drupal_set_message("Successfuly Deleted Object: {$this->pid}");
      return array('success' => true, 'msg' => 'Successfuly Deleted Object');
    }
    else {
      return array('success' => false, 'msg' => 'Failed to Delete Object');
    }
  }

  /**
   * Gets the fedora object's label.
   *
   * @return string
   */
  private function getObjectLabel() {
    return $this->item->objectProfile->objLabel;
  }

  /**
   * Gets the fedora object's creation date in the format "Month Day, Year".
   *
   * @return string
   */
  private function getObjectCreationDate() {
    $date_created = new DateTime($this->item->objectProfile->objCreateDate);
    return $date_created->format('F j, Y');
  }

  /**
   * Gets the fedora object's last modified date in the format "Month Day, Year".
   *
   * @return string
   */
  private function getObjectLastModifiedDate() {
    $date_modified = new DateTime($this->item->objectProfile->objLastModDate);
    return $date_modified->format('F j, Y');
  }

  /**
   * Gets the fedora object's owner id.
   *
   * @return string
   */
  private function getObjectOwner() {
    $xpath = $this->getFoxmlXPath();
    $result = $xpath->query("/foxml:digitalObject/foxml:objectProperties/foxml:property[@NAME='info:fedora/fedora-system:def/model#ownerId']/@VALUE");
    if ($result && $result->length == 1) {
      return $result->item(0)->value;
    }
    return '';
  }

  /**
   * Gets the fedora object's current state.
   *
   * @return string
   */
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
