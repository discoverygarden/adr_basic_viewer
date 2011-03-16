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
    return 'PDF';//$keys[0];
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

