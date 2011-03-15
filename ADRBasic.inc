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
  protected $titles;
  protected $authors;
  protected $description;
  protected $dateCreated;
  protected $dateModified;
  protected $user;
  protected $defaultMimeIcons = array(
    'application/rdf+xml' => 'pdf.png',
    'text/xml' => 'pdf.png',
    'image/jpeg' => 'pdf.png',
  );

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
    $this->titles = $mods->xpath('/mods:mods/mods:titleInfo/mods:title');
    $this->authors = $mods->xpath('//mods:role[mods:roleTerm = "creator"]/../../mods:name/mods:namePart');
    $this->description = $mods->xpath('//mods:abstract');
    $foxml = new SimpleXMLElement($this->item->export_as_foxml());
    $foxml->registerXPathNamespace("audit", "info:fedora/fedora-system:def/audit#");
    $audit = $foxml->xpath("(//*[local-name()='responsibility'])[last()]");
    $this->user = $audit[0];
    $this->dateCreated = new DateTime($this->item->objectProfile->objCreateDate);
    $this->dateCreated = $this->dateCreated->format('F j, Y');
    $this->dateModified = new DateTime($this->item->objectProfile->objLastModDate);
    $this->dateModified = $this->dateModified->format('F j, Y');
  }

  /**
   * Determines if the user can view the manage tab, this is based on drupal/fedora permissions.
   *
   * @return boolean
   */
  private function canManageObject() {
    return FALSE;
  }

  private function includeRequiredScripts() {
    $path = drupal_get_path('module', 'fedora_repository');
    drupal_add_css("$path/stylesheets/ADRBasic.css");
    module_load_include('module', 'ext', 'ext');
    ext_load_library();
    /*$this->createDescriptionStore();
    $this->createDatastreamStore();*/
    $include_files = array(
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
      'ADRBasic.js',
      'Description.js',
      'Datastreams.js'
    );
    foreach ($include_files as $file) {
      drupal_add_js("$path/js/ADRBasic/$file");
    }
  }

  private function createDescriptionStore() {
    $data = array();
    $fields = array('title', 'author', 'description');
    $data[] = array(
      implode(' ', $this->titles), implode(' ', $this->authors), $this->description
    );
    $fields = $this->convertPhpArrayToJsString($fields);
    $data = $this->convertPhpArrayToJsString($data);
    $datastream = $this->createStore('FedoraObjectInfoStore', $data, $fields);
    drupal_add_js($datastream, 'inline');
  }

  private function createDatastreamStore() {
    $data = array();
    $fields = array('dsid', 'label', 'mime', 'img');
    foreach ($this->item->datastreams as $dsid => $attr) {
      $img = $this->getDatastreamIconURL($dsid, $attr['MIMEType']);
      $data[] = array($dsid, $attr['label'], $attr['MIMEType'], $img);
    }
    $fields = $this->convertPhpArrayToJsString($fields);
    $data = $this->convertPhpArrayToJsString($data);
    $datastream = $this->createStore('DatastreamStore', $data, $fields);
    drupal_add_js($datastream, 'inline');
  }

  private function getDatastreamIconURL($dsid, $mime_type) {
    // Find in object
    // If not found use default for mime type.
    global $base_url;
    $path_to_default_images = drupal_get_path('module', 'adr_basic_viewer') . '/images/';
    $image = $this->defaultMimeIcons[$mime_type];
    return "$base_url/$path_to_default_images$image"; // $image;
  }

  private function createStore($storeId, $data, $fields) {
    return "$storeId = Ext.extend(Ext.data.ArrayStore, {
    constructor: function(cfg) {
        cfg = cfg || {};
        $storeId.superclass.constructor.call(this, Ext.apply({
            storeId: '$storeId',
            idIndex: 0,
            autoDestroy: true,
            data: $data,
            fields: $fields}, cfg));
    }});
    new $storeId()";
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
    module_load_include('inc', 'fedora_repository', 'plugins/ShowStreamsInFieldSets');
    module_load_include('inc', 'fedora_repository', 'plugins/tagging_form');
    $this->includeRequiredScripts();
  }

}