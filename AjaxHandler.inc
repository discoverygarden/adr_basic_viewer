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
   * This fedora objects content model.
   *
   * Access through the function AjaxHandler::getContentModel().
   * Do not directly as it may not exist.
   *
   * @var ContentModel
   */
  protected $contentModel;

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
   *
   * @return array
   */
  public function getAllowedMIMETypes() {
    $content_model = &$this->getContentModel();
    $mime_types = $content_model->getMimetypes();
    $response = array();
    foreach ($mime_types as $type) {
      $response[] = array('mime' => $type);
    }
    return $response;
  }

  /**
   *
   * @return array
   */
  public function getDescription() {
    module_load_include('inc', 'adr_basic_viewer', 'ObjectViewDescription');
    $object_view = new ObjectViewDescription($this->item, $this->getContentModel());
    $description = $object_view->getDescription();
    $content = '';
    foreach ($description as $label => $value) {
      $content .= '<tr>';
      $content .= "<td style='padding:5px;font-size:1.5em;'>$label</td>";
      $content .= "<td style='padding:5px;font-size:1.2em;'>$value</td>";
      $content .= '</tr>';
    }
    $created = $this->getObjectCreationDate();
    $modified = $this->getObjectLastModifiedDate();
    $user = $this->getLastUserToModifyObject();
    // @TODO use classes instead of inlined values.
    $footer = "<tr><td colspan='2'><br/></td></tr>";
    $footer .= "<tr><td colspan='2' style='text-align: center;font-size:0.9em;'>";
    $footer .= "This object was added {$created}. ";
    $footer .= "This object was last updated {$modified} by {$user}.</td></tr>";
    return "<table>{$content}{$footer}</table>";
  }

  /**
   *
   * @return array
   */
  public function getDatastreams($start, $limit) {
    $data = array();
    $count = 0;
    $end = $start + $limit;
    foreach ($this->item->datastreams as $dsid => $attr) {
      if ($count >= $start && $count < $end) {
        module_load_include('inc', 'adr_basic_viewer', 'Thumbnail');
        $thumbnail = new Thumbnail($this->item, $dsid);
        $stream['dsid'] = $dsid;
        $stream['label'] = $attr['label'];
        $stream['img_url'] = $thumbnail->getUrl();
        $stream['download_url'] = $this->getDatastreamDownloadURL($dsid, $attr['label']);
        $stream['mime'] = $attr['MIMEType'];
        $data[] = $stream;
      }
      $count++;
    }
    return array("success" => TRUE, "data" => $data, "total" => $count);
  }

  /**
   *
   * @param int $start
   * @param int $limit
   * @return array
   */
  public function getOverviewDatastreams($start, $limit) {
    $data = array();
    module_load_include('inc', 'adr_basic_viewer', 'ObjectViewDescription');
    $object_view = new ObjectViewDescription($this->item, $this->getContentModel());
    $count = 0;
    $end = $start + $limit;
    foreach ($this->item->datastreams as $dsid => $attr) {
      $should_add = $object_view->canViewDatastream($dsid);
      if ($should_add) {
        if ($count >= $start && $count < $end) {
          module_load_include('inc', 'adr_basic_viewer', 'Thumbnail');
          $thumbnail = new Thumbnail($this->item, $dsid);
          $stream['dsid'] = $dsid;
          $stream['label'] = $attr['label'];
          $stream['img_url'] = $thumbnail->getUrl();
          $stream['download_url'] = $this->getDatastreamDownloadURL($dsid, $attr['label']);
          $stream['mime'] = $attr['MIMEType'];
          $data[] = $stream;
        }
        $count++;
      }
    }
    return array("success" => TRUE, "data" => $data, "total" => $count);
  }

  /**
   *
   * @global string $base_url
   * @param string $dsid
   */
  private function getDatastreamDownloadURL($dsid, $label) {
    global $base_url;
    return "$base_url/fedora/repository/object_download/" . drupal_urlencode($this->pid) . '/' . $dsid . '/' . drupal_urlencode(preg_replace('/\//i', '${1}_', $label));
  }

  /**
   *
   * @param <type> $values
   * @return <type> 
   */
  public function addDatastream($values) {
    $controls = array(
      'Inline XML (X)' => "X",
      'Managed Resource (M)' => "M",
      'Redirect (R)' => "R",
      'External Reference (E)' => "E",
    );
    $dsid = $values['dsid'];
    $label = $values['label'];
    $control = $controls[$values['control_group']];
    $mime = $values['mime_type'];
    $file = array_shift($_FILES);
    if ($file['type'] != $mime) {
      return array('success' => false, 'msg' => 'Mime type of file does not match the mime type specified.');
    }
    else {
      if ($this->item->add_datastream_from_file($file['tmp_name'], $dsid, $label, $mime, $control)) {
        return array('success' => true, 'msg' => 'Successfully added datastream.');
      }
      else {
        return array('success' => false, 'msg' => 'Failed to add datastream.');
      }
    }
  }

  /**
   * Deletes datastreams from the object.
   *
   * @param string $dsid
   * @return array
   */
  public function deleteDatastream($dsid) {
    if ($this->item->purge_datastream($dsid)) {
      return json_encode(array('success' => true, 'msg' => 'Successfully Deleted datastream.'));
    }
    else {
      return json_encode(array('success' => false, 'msg' => 'Failed to deleted file.'));
    }
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
   * 
   */
  private function getLastUserToModifyObject() {
    $xpath = $this->getFoxmlXPath();
    $result = $xpath->query("(//*[local-name()='responsibility'])[last()]");
    $audit = $result->item(0)->nodeValue;
    return sprintf("%s", $audit);
  }

  /**
   * 
   *
   * @return ContentModel
   */
  private function &getContentModel() {
    if (empty($this->contentModel)) {
      module_load_include('inc', 'fedora_repository', 'ContentModel');
      $this->contentModel = ContentModel::loadFromObject($this->pid);
    }
    return $this->contentModel;
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
