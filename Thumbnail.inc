<?php

// $Id$

/**
 * @file
 *
 */
class Thumbnail {

  /**
   * This datastreams id.
   *
   * @var string
   */
  protected $dsid;
  /**
   * The datastreams mime type.
   *
   * @var string
   */
  protected $mime_type;
  /**
   * The fedora object.
   *
   * @var fedora_item
   */
  protected $item;
  /**
   * The url to this datastream's thumbnail.
   *
   * @var string
   */
  protected $url;
  /**
   * The default icon for a datastream. This is used when there is no thumbnail associated with the datastream.
   * The thumbnail will be choosen based on the datastreams mime type.
   * 
   * @var array
   */
  protected $defaultMimeIcons = array(
    'application/pdf' => 'application-pdf.png',
    'application/rdf+xml' => 'text-txt.png',
    'application/xml' => 'text-txt.png',
    'text/xml' => 'text-txt.png',
    'audio/midi' => 'audio.png',
    'audio/x-aiff' => 'audio.png',
    'audio/x-mpeg' => 'audio.png',
    'audio/x-wav' => 'audio.png',
    'audio/vnd.wave' => 'audio.png',
    'audio/x-pn-realaudio' => 'audio.png',
    'audio/mpeg' => 'audio.png',
    'audio/mp3' => 'audio.png',
    'audio/wav' => 'audio.png',
    'video/x-msvideo' => 'video.png',
    'video/x-flv' => 'video.png',
    'video/mpeg' => 'video.png',
    'video/mp4' => 'video.png',
    'video/quicktime' => 'video.png',
  );
  /**
   * The default icon for a datastream. This is used when there is no thumbnail associated with the datastream,
   * and the datastreams mime type doesn't have a default icon.
   *
   * @var string
   */
  protected $defaultIcon = 'default.png';

  /**
   *
   * @param Fedora_Item $fedora_item
   * @param string $dsid
   */
  public function __construct(&$fedora_item, $dsid) {
    $this->item = &$fedora_item;
    $this->dsid = $dsid;
    $this->mime_type = $this->item->datastreams[$dsid]['MIMEType'];
  }

  /**
   * Gets the url to this datastreams thumbnail.
   *
   * @return string
   */
  public function getUrl() {
    if (empty($this->url)) {
      if ($this->isIcon()) {
        $this->url = $this->getDatastreamUrl($this->dsid);
      }
      else {
        $this->url = $this->getUrlFromRelsInt();
        if (empty($this->url)) {
          $this->url = $this->getUrlFromName();
          if (empty($this->url)) {
            $this->url = $this->getDefaultIconUrl();
          }
        }
      }
    }
    return $this->url;
  }

  /**
   * Determines if this datastram is a thumbnail
   *
   * @return boolean
   */
  private function isIcon() {
    return ($this->dsid == 'TN') || (preg_match('/-tn\.([^\/]*)$/i', $this->dsid) > 0);
  }

  /**
   * Gets a url to a given datastream.
   *
   * @global $base_url
   * @param string $dsid
   * @return string
   */
  private function getDatastreamUrl($dsid) {
    global $base_url;
    return "$base_url/fedora/repository/{$this->item->pid}/{$dsid}";
  }

  /**
   * Gets the url from rels int if it exists, and is defined in the rels int.
   *
   * @return string
   */
  private function getUrlFromRelsInt() {
    $rels_int = $this->getRelsInt();
    if (isset($rels_int)) {
      $path = "/rdf:RDF/rdf:Description[@rdf:about='info:fedora/{$this->item->pid}/{$this->dsid}']/coal:hasThumbnail"; // coal Namespace is worry some.
      $results = $rels_int->xpath($path);
      if (is_array($results) && count($results) > 0) {
        $attributes = $results[0]->attributes('rdf', TRUE);
        if (isset($attributes['resource'])) {
          $dsid = sprintf("%s", $attributes['resource']);
          $dsid = preg_filter('/^.*\/([^\/]*)$/', '$1', $dsid);
          return $this->getDatastreamUrl($dsid);
        }
      }
    }
    return NULL;
  }

  /**
   * Gets the RELS-INT datastream as a SimpleXMLElement.
   *
   * @return SimpleXMLElement 
   */
  private function getRelsInt() {
    if(isset($this->item->datastreams['RELS-INT'])) {
      $rels_int = $this->item->get_datastream_dissemination("RELS-INT");
      if (trim($rels_int) != '') {
        return new SimpleXMLElement($rels_int);
      }
    }
    return NULL;
  }

  /**
   * Gets the url from this datastreams dsid, if it exists.
   * 
   * @global string $base_url
   * @return string
   */
  private function getUrlFromName() {
    global $base_url;
    
    if (preg_match('/\..+$/i', $this->dsid) > 0) { // Has extension.
      preg_match('/(.*)\/(.*)/', $this->mime_type, $matches);
      if ($matches[1] != 'audio') {
        $dsid = preg_replace('/\..+$/i', '-tn.jpg', $this->dsid);
        if (array_key_exists($dsid, $this->item->datastreams)) {
          return $this->getDatastreamUrl($dsid);
        }
      }
    }
    return NULL;
  }

  /**
   * Gets the url for the default icon based on mime type.
   *
   * @return string
   */
  private function getDefaultIconUrl() {
    $default_image_path = drupal_get_path('module', 'adr_basic_viewer') . '/images/mime';
    $image = isset($this->defaultMimeIcons[$this->mime_type]) ? $this->defaultMimeIcons[$this->mime_type] : $this->defaultIcon;
    return "$base_url/$default_image_path/$image";
  }

}
