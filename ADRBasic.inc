<?php

// $Id$

/**
 * @file
 *
 * Basic viewer for Colorado Alliance style Fedora Objects.
 */

/**
 * A viewer for Fedora Objects
 */
class ADRBasic {

  /**
   * The PID of the object to be viewed/managed.
   * 
   * @var string
   */
  protected $pid;
  /**
   * The object the user will be viewing
   *
   * @var Fedora_Item
   */
  protected $item;
  /**
   * MODS datastream. Information is extracted from this XML document and rendered to the form.
   * 
   * @var array
   */
  protected $mods;
  protected $title;
  protected $author;
  protected $description;
  protected $dateCreated;
  protected $dateModified;
  protected $user;
  protected $defaultMimeIcons = array(
    'application/pdf' => 'pdf.png',
    'application/rdf+xml' => 'pdf.png',
    'text/xml' => 'pdf.png',
    'image/jpeg' => 'pdf.png',
    'video/x-flv' => 'pdf.png'
  );
  protected $defaultIcon = 'default.png';

  /**
   * Create a view for describing/managing an object.
   *
   * @param string $pid
   */
  public function __construct($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->pid = $pid;
    $this->item = new Fedora_Item($this->pid);
    $this->setDisplayInfo();
  }

  /**
   *
   */
  private function setDisplayInfo() {
    $mods = new SimpleXMLElement($this->item->get_datastream_dissemination("MODS"));
    $mods->registerXPathNamespace("mods", "http://www.loc.gov/mods/v3");
    $titles = $mods->xpath('/mods:mods/mods:titleInfo/mods:title');
    $this->title = $this->implodeSimpleXMLElementArray(', ', $titles);
    $authors = $mods->xpath('//mods:role[mods:roleTerm = "creator"]/../../mods:name/mods:namePart');
    $this->author = $this->implodeSimpleXMLElementArray(', ', $authors);
    $description = $mods->xpath('//mods:abstract');
    $this->description = sprintf("%s", $description[0]);

    $foxml = new SimpleXMLElement($this->item->export_as_foxml());
    $foxml->registerXPathNamespace("audit", "info:fedora/fedora-system:def/audit#");
    $audit = $foxml->xpath("(//*[local-name()='responsibility'])[last()]");
    $this->user = sprintf("%s", $audit[0]);
    $this->dateCreated = new DateTime($this->item->objectProfile->objCreateDate);
    $this->dateCreated = $this->dateCreated->format('F j, Y');
    $this->dateModified = new DateTime($this->item->objectProfile->objLastModDate);
    $this->dateModified = $this->dateModified->format('F j, Y');
  }

  /**
   * Responds to a request for a fedora objects description.
   * 
   * @return string
   *   JSON encoded string.
   */
  public function getDescriptionAjaxResponse() {
    $response = array();
    $response['title'] = $this->title;
    $response['author'] = $this->author;
    $response['description'] = $this->description;
    $response['date_created'] = $this->dateCreated;
    $response['date_modified'] = $this->dateModified;
    $response['user'] = $this->user;
    return json_encode(array($response)); // One record, within an array.
  }

  public function setDescriptionAjaxRequest() {
    $this->title = $_POST['title'];
    $this->author = $_POST['author'];
    $this->description = $_POST['description'];
    $data = array(
      'title' => $this->title,
      'author' => $this->author,
      'description' => $this->description,
      'date_created' => $this->dateCreated,
      'date_modified' => $this->dateModified,
      'user' => $this->user
    );
    return json_encode(array('success' => true, 'msg' => 'Successfully updated description.', 'data' => array($data)));
  }

  public function getAllowedMIMETypesAjaxResponse() {
    module_load_include('inc', 'fedora_repository', 'ContentModel');
    $content_model = ContentModel::loadFromObject($this->pid);
    $mime_types = $content_model->getMimetypes();
    $response = array();
    foreach($mime_types as $type) {
      $response[] = array('mime' => $type);
    }
    return json_encode($response);
  }

  public function getDatastreamsAjaxResponse() {
    $response = array();
    foreach ($this->item->datastreams as $dsid => $attr) {
      $img = $this->getDatastreamIconURL($dsid, $attr['MIMEType']);
      $stream['dsid'] = $dsid;
      $stream['label'] = $attr['label'];
      $stream['img_url'] = $img;
      $stream['download_url'] = $this->getDatastreamDownloadURL($dsid, $attr['label']);
      $stream['mime'] = $attr['MIMEType'];
      $response[] = $stream;
    }
    return json_encode($response);
  }

  /**
   *
   * @param <type> $var
   * @param <type> $values
   */
  private function implodeSimpleXMLElementArray($glue, $values) {
    $output = '';
    if (isset($values)) {
      while (($value = array_shift($values)) != NULL) {
        $value = sprintf("%s", $value);
        if (trim($value) != '') {
          $output .= ( $output != '') ? $glue : '';
          $output .= $value;
        }
      }
    }
    return $output;
  }

  /**
   *
   * @global $base_url $base_url
   * @param <type> $dsid 
   */
  private function getDatastreamDownloadURL($dsid, $label) {
    global $base_url;
    return "$base_url/fedora/repository/object_download/" . drupal_urlencode($this->pid) . '/' . $dsid . '/' . drupal_urlencode(preg_replace('/\//i', '${1}_', $label));
  }

  /**
   *
   * @global  $base_url
   * @param <type> $dsid
   * @param <type> $mime_type
   * @return <type> 
   */
  private function getDatastreamIconURL($dsid, $mime_type) {
    // Find in object
    // If not found use default for mime type.
    global $base_url;
    $path_to_default_images = drupal_get_path('module', 'adr_basic_viewer') . '/images/';
    $image = isset($this->defaultMimeIcons[$mime_type]) ? $this->defaultMimeIcons[$mime_type] : $this->defaultIcon;
    return "$base_url/$path_to_default_images$image"; // $image;
  }

  /**
   * Determines if the user can view the manage tab, this is based on drupal/fedora permissions.
   *
   * @return boolean
   */
  private function canManageObject() {
    return FALSE;
  }

  /**
   * 
   */
  private function includeRequiredScripts() {
    $path = drupal_get_path('module', 'adr_basic_viewer');
    drupal_add_css("$path/stylesheets/ADRBasic.css");
    module_load_include('inc', 'adr_basic_viewer', 'ADRBasicViewer');
    //ADRBasicViewer::includeRequiredJSForViewers($this->pid);
    module_load_include('module', 'ext', 'ext');
    ext_load_library();
    $include_files = array(
      'Description.js',
      'Datastreams.js',
      'ControlGroups.js',
      'MIMETypes.js',
      'EditObjectWindow.ui.js',
      'EditObjectWindow.js',
      'DeleteObjectWindow.ui.js',
      'DeleteObjectWindow.js',
      'AddFileWindow.ui.js',
      'AddFileWindow.js',
      'EditFileWindow.ui.js',
      'EditFileWindow.js',
      'ADRBasicViewer.ui.js',
      'ADRBasicViewer.js',
      'OverviewPanel.ui.js',
      'OverviewPanel.js',
      'ViewerPanel.ui.js',
      'ViewerPanel.js',
      'ManagePanel.ui.js',
      'ManagePanel.js',
      'EditObjectPanel.ui.js',
      'EditObjectPanel.js',
      'EditDescriptionPanel.ui.js',
      'EditDescriptionPanel.js',
      'EditFilesPanel.ui.js',
      'EditFilesPanel.js',
      'DescriptionPanel.ui.js',
      'DescriptionPanel.js',
      'FilesPanel.ui.js',
      'FilesPanel.js',
      'ADRBasic.js'
    );
    foreach ($include_files as $file) {
      drupal_add_js("$path/js/$file");
    }
  }

  /**
   *
   * @param <type> $array
   * @return string
   */
  private function convertPhpArrayToJsString($array) {
    $output = '[ ';
    foreach ($array as $value) {
      if (is_array($value)) {
        $output .= $this->convertPhpArrayToJsString($value) . ',';
      }
      else {
        $output .= "'$value',";
      }
    }
    $output .= ' ]';
    return $output;
  }

  /**
   * Render a set of tabs that describe the object and in some cases allow it to be modified by a user.
   * @global <type> $base_url
   * @global <type> $user
   * @return <type>
   */
  public function render() {
    $this->includeRequiredScripts();
  }

}