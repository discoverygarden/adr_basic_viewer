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
  protected $relsInt;
  protected $title;
  protected $author;
  protected $description;
  protected $dateCreated;
  protected $dateModified;
  protected $user;
  protected $defaultMimeIcons = array(
    'application/pdf' => 'application-pdf.png',
    'application/rdf+xml' => 'text-txt.png',
    'text/xml' => 'text-txt.png',
    'image/jpeg' => 'image-default.png',
    'video/x-flv' => 'video-x-generic-mplayer.png'
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
    $this->mods = new SimpleXMLElement($this->item->get_datastream_dissemination("MODS"));
    $rels_int = $this->item->get_datastream_dissemination("RELS-INT");
    if(trim($rels_int) != '') {
      $this->relsInt = new SimpleXMLElement($rels_int);
    }
    $this->setDisplayInfo();
  }

  /**
   *
   */
  private function setDisplayInfo() {
    $mods = $this->mods;
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
    /* $response = array();
      $response['title'] = $this->title;
      $response['author'] = $this->author;
      $response['description'] = $this->description;
      $response['date_created'] = $this->dateCreated;
      $response['date_modified'] = $this->dateModified;
      $response['user'] = $this->user;
      return json_encode(array($response)); */// One record, within an array.
    $rows = array(
      'Title' => '/mods:mods/mods:titleInfo[not(@type)]/mods:title',
      'Alternative Title' => '/mods:mods/mods:titleInfo[@type="alternative"]/mods:title',
      'Family Name' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="creator"]/..)/mods:namePart[not(@type)]',
      'Given Name' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="creator"]/..)/mods:namePart[@type="family"]',
      'Terms of Address' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="creator"]/..)/mods:namePart[@type="given"]',
      'Family Name' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="creator"]/..)/mods:namePart[@type="termsOfAddress"]',
      'Given Name' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="contributor"]/..)/mods:namePart[not(@type)]',
      'Terms of Address' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="contributor"]/..)/mods:namePart[@type="family"]',
      'Corporate Creator' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="contributor"]/..)/mods:namePart[@type="given"]',
      'Corporate Contributor' => '(/mods:mods/mods:name[@type="personal"]/mods:role[mods:roleTerm="contributor"]/..)/mods:namePart[@type="termsOfAddress"]',
      'Type of Resource' => '(/mods:mods/mods:name[@type="corporate"]/mods:role[mods:roleTerm="creator"]/..)/mods:namePart[not(@type)]',
      'Origin Information' => '(/mods:mods/mods:name[@type="corporate"]/mods:role[mods:roleTerm="contributor"]/..)/mods:namePart[not(@type)]',
      'Place of Publication' => '/mods:mods/mods:typeOfResource',
      'Publisher' => '/mods:mods/mods:originInfo/mods:place/mods:placeTerm[@type="text"]',
      'Date Issued' => '/mods:mods/mods:originInfo/mods:publisher',
      'Date Created' => '/mods:mods/mods:originInfo/mods:dateIssued',
      'Copyright Date' => '/mods:mods/mods:originInfo/mods:dateCreated',
      'Other Date' => '/mods:mods/mods:originInfo/mods:copyrightDate',
      'Genre' => '/mods:mods/mods:originInfo/mods:dateOther',
      'Language' => '/mods:mods/mods:genre[@type="marcgt"]',
      'Physical Description' => '/mods:mods/mods:language',
      'Extent' => '/mods:mods/mods:physicalDescription/mods:extent',
      'Form' => '/mods:mods/mods:physicalDescription/mods:form',
      'Digitial Origin' => '/mods:mods/mods:physicalDescription/mods:digitalOrigin',
      'Physical Node' => '/mods:mods/mods:physicalDescription/mods:note',
      'Description' => '/mods:mods/mods:abstract',
      'Target Audience' => '/mods:mods/mods:targetAudience',
      'Notes' => '/mods:mods/mods:note[not(@type)]',
      'Administrative Notes' => '/mods:mods/mods:note[@type="admin"]',
      'Funding Notes' => '/mods:mods/mods:note[@type="funding"]',
      'Topics' => '/mods:mods/mods:subject/mods:topic',
      'Places' => '/mods:mods/mods:subject/mods:geographic',
      'Dates' => '/mods:mods/mods:subject/mods:temporal',
      'People' => '/mods:mods/mods:subject/mods:name[@type="personal"]',
      'Organizations' => '/mods:mods/mods:subject/mods:name[@type="corporate"]',
      'Host Object' => '/mods:mods/mods:relatedItem[@type="host"]/mods:titleInfo/mods:title',
      'Constituent Object' => '/mods:mods/mods:relatedItem[@type="constituent"]/mods:titleInfo/mods:title',
      'Series' => '/mods:mods/mods:relatedItem[@type="series"]/mods:titleInfo/mods:title',
      'Local Identifier' => '/mods:mods/mods:identifier[@type="local"]',
      'ISBN' => '/mods:mods/mods:identifier[@type="isbn"]',
      'ISSN' => '/mods:mods/mods:identifier[@type="issn"]',
      'Handle' => '/mods:mods/mods:location/mods:url[@usage="primary display"]',
      'Rights Statement' => '/mods:mods/mods:accessCondition[@type="useAndReproduction"]'
    );
    $content = '';
    foreach ($rows as $label => $path) {
      $value = $this->getValueFromPath($path);
      if ($value != '') {
        $content .= $this->createRow($label, $value);
      }
    }
    $footer = "<tr><td colspan='2'><br/></td></tr>";
    $footer .= "<tr><td colspan='2' style='text-align: center;font-size:0.9em;'>This object was added {$this->dateCreated}. This object was last updated {$this->dateModified} by {$this->user}.</td></tr>";
    $html = <<<EOT
    <table>
      {$content}
      {$footer}
    </table>
EOT;
    return $html;
  }

  private function getValueFromPath($path) {
    $value = $this->mods->xpath($path);
    return trim($this->implodeSimpleXMLElementArray(', ', $value));
  }

  private function createRow($label, $value) {
    return <<<EOD
      <tr>
        <td style="padding:5px;font-size:1.5em;">$label</td>
        <td style="padding:5px;font-size:1.2em;">$value</td>
      </tr>
      
EOD;
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

  public function addDatastreamAjaxRequest() {
    $controls = array(
      'Inline XML (X)' => "X",
      'Managed Resource (M)' => "M",
      'Redirect (R)' => "R",
      'External Reference (E)' => "E",
    );
    $dsid = $_POST['dsid'];
    $label = $_POST['label'];
    $control = $controls[$_POST['control_group']];
    $mime = $_POST['mime_type'];
    $state = $_POST['state'];
    $file = array_shift($_FILES);
    if ($file['type'] != $mime) {
      return json_encode(array('success' => false, 'msg' => 'Mime type of file does not match the mime type specified.'));
    }
    else {
      if ($this->item->add_datastream_from_file($file['tmp_name'], $dsid, $label, $mime, $control)) {
        return json_encode(array('success' => true, 'msg' => 'Successfully added datastream.'));
      }
      else {
        return json_encode(array('success' => false, 'msg' => 'Failed to add datastream.'));
      }
    }
  }

  public function getAllowedMIMETypesAjaxResponse() {
    module_load_include('inc', 'fedora_repository', 'ContentModel');
    $content_model = ContentModel::loadFromObject($this->pid);
    $mime_types = $content_model->getMimetypes();
    $response = array();
    foreach ($mime_types as $type) {
      $response[] = array('mime' => $type);
    }
    return json_encode($response);
  }

  public function getDatastreamsAjaxResponse() {
    $response = array();
    foreach ($this->item->datastreams as $dsid => $attr) {
      $img = $this->getThumbnail($dsid, $attr['MIMEType']);
      $stream['dsid'] = $dsid;
      $stream['label'] = $attr['label'];
      $stream['img_url'] = $img;
      $stream['download_url'] = $this->getDatastreamDownloadURL($dsid, $attr['label']);
      $stream['mime'] = $attr['MIMEType'];
      $stream['overview'] = self::canShowDatastreamInOverview($dsid, $attr);
      $response[] = $stream;
    }
    return json_encode($response);
  }

  public function getOverviewDatastreamsAjaxResponse() {
    $response = array();
    foreach ($this->item->datastreams as $dsid => $attr) {
      $should_add = self::canShowDatastreamInOverview($dsid, $attr);
      if ($should_add) {
        $img = $this->getThumbnail($dsid, $attr['MIMEType']);
        $stream['dsid'] = $dsid;
        $stream['label'] = $attr['label'];
        $stream['img_url'] = $img;
        $stream['download_url'] = $this->getDatastreamDownloadURL($dsid, $attr['label']);
        $stream['mime'] = $attr['MIMEType'];
        $stream['overview'] = $should_add;
        $response[] = $stream;
      }
    }
    return json_encode($response);
  }

  static public function canShowDatastreamInOverview($dsid, $attributes) {
    $mime_types_allowed_in_overview = array(
      'application/pdf', 'image/jpeg', 'image/jp2', 'image/tiff', 'image/png', 'video/mp4'
    );
    $mime = $attributes['MIMEType'];
    $mime_allowed = array_search($mime, $mime_types_allowed_in_overview) !== FALSE;
    $is_thumbnail = ($dsid == 'TN') || (preg_match("/tn\.jpg/", $dsid) > 0);
    return $mime_allowed && !$is_thumbnail;
  }

  /**
   *
   * @param <type> $var
   * @param <type> $values
   */
  private function implodeSimpleXMLElementArray($glue, $values) {
    $output = '';
    if (isset($values) && $values !== FALSE) {
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
   * @return string
   */
  private function getThumbnail($dsid, $mime_type) {
    global $base_url;
    $thumbnail = $this->isIcon($dsid) ? "$base_url/fedora/repository/{$this->pid}/$dsid" : FALSE;
    $thumbnail = empty($thumbnail) ? $this->getThumbnailFromRELSINT($dsid) : $thumbnail;
    $thumbnail = empty($thumbnail) ? $this->getThumbnailFromName($dsid) : $thumbnail;
    if($thumbnail) {
      return $thumbnail;
    }
    else {
      $default_image_path = drupal_get_path('module', 'adr_basic_viewer') . '/images/mime';
      $image = isset($this->defaultMimeIcons[$mime_type]) ? $this->defaultMimeIcons[$mime_type] : $this->defaultIcon;
      return "$base_url/$default_image_path/$image";
    }
  }

  private function isIcon($dsid) {
    return ($dsid == 'TN') || (preg_match('/-tn\.jpg/i', $dsid) > 0);
  }

  private function getThumbnailFromRELSINT($dsid) {
    global $base_url;
    if(!isset($this->relsInt))
        return FALSE;
    $path = "/rdf:RDF/rdf:Description[@rdf:about='info:fedora/{$this->pid}/{$dsid}']/coal:hasThumbnail";
    $results = $this->relsInt->xpath($path);
    if (is_array($results) && count($results) > 0) {
      $attributes = $results[0]->attributes('rdf', TRUE);
      if (isset($attributes['resource'])) {
        $thumbnail = sprintf("%s", $attributes['resource']);
        $thumbnail = preg_filter('/^.*\/([^\/]*)$/', '$1', $thumbnail);
        return "$base_url/fedora/repository/{$this->pid}/$thumbnail";
      }
    }
    return FALSE;
  }

  private function getThumbnailFromName($dsid) {
    global $base_url;
    if (preg_match('/\..+$/i', $dsid) > 0) { // Has extension.
      $thumbnail = preg_replace('/\..+$/i', '-tn.jpg', $dsid);
      if (array_key_exists($thumbnail, $this->item->datastreams)) {
        return "$base_url/fedora/repository/{$this->pid}/$thumbnail";
      }
    }
    return FALSE;
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
    module_load_include('module', 'ext', 'ext');
    ext_load_library();
    drupal_add_css("$path/stylesheets/ADRBasic.css", 'theme', 'all');
    module_load_include('inc', 'adr_basic_viewer', 'ADRBasicViewer');
    ADRBasicViewer::includeRequiredJSForViewers($this->pid);
    $include_files = array(
      'IncludeFirst.js',
      'Description.js',
      'Datastreams.js',
      'OverviewDatastreams.js',
      'ControlGroups.js',
      'MIMETypes.js',
      'FileUploadField.ui.js',
      'FileUploadField.js',
      'AddFileWindow.ui.js',
      'AddFileWindow.js',
      'ReplaceFileWindow.ui.js',
      'ReplaceFileWindow.js',
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