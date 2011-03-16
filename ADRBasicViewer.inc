<?php

// $Id$

/**
 * @file
 *
 */
class ADRBasicViewer {

  protected $pid;
  protected $dsid;
  protected $item;
  protected $mimeType;

  public static function includeRequiredJSForViewers($pid) {
    self::includeJSForFlash($pid);
  }

  private static function includeJSForFlash($pid) {
    $path = drupal_get_path('module', 'Fedora_Repository');
    $full_path = base_path() . $path;
    $path_to_js = "$path/js/swfobject.js";
    drupal_add_js("$path_to_js");
    drupal_add_js("var s1 = new SWFObject('$full_path/flash/flvplayer.swf', 'single', '320', '240', '7');
    s1.addParam('allowfullscreen', 'TRUE');
    s1.addVariable('file', '". base_path() . "fedora/repository/$pid/FLV/FLV.flv');
    s1.write('player{$pid}FLV');", 'inline', 'footer');
  }

  public function __construct($pid, $dsid = NULL) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->item = new Fedora_Item($pid);
    $this->pid = $pid;
    $this->dsid = isset($dsid) ? $dsid : $this->getDefaultDsid();
    $this->mimeType = $this->item->datastreams[$this->dsid]['MIMEType'];
  }

  /**
   *
   * @return string
   */
  private function getDefaultDsid() {
    $keys = array_keys($this->item->datastreams);
    return $keys[0];
  }

  /**
   *
   * @return string
   */
  public function getViewer() {
    global $base_url;
    switch ($this->mimeType) {
      case 'application/rdf+xml':
      case 'text/xml':
        $xmlstr = $this->item->get_datastream_dissemination($this->dsid);
        $content = htmlentities($xmlstr);
        return "<div>$content</div>";
      case 'application/pdf':
        return "<iframe src='http://docs.google.com/viewer?url=$base_url/fedora/repository/{$this->pid}/{$this->dsid}/&embedded=TRUE' style='width:960px; height:953px;' frameborder='0'></iframe>";
      case 'image/jpeg':
        return "<a href='$base_url/fedora/repository/{$this->pid}/{$this->dsid}/'><img src='$base_url/fedora/repository/{$this->pid}/{$this->dsid}'/></a>";
      default:
        return '<div>There is no viewer for the selected file type.</div>';
    }
  }

}

