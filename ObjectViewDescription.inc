<?php

// $Id$

/**
 * @file
 *
 */
class ObjectViewDescription {
  const mods = 'http://www.loc.gov/standards/mods/v3';
  const xlink = 'http://www.w3.org/1999/xlink';
  const xsi = 'http://www.w3.org/2001/XMLSchema-instance';
  const xmlns = 'http://www.w3.org/2000/xmlns/';
  /**
   *
   * @var <type>
   */
  protected $item;
  /**
   *
   * @var <type>
   */
  protected $objectViewDescription;
  /**
   *
   * @var <type>
   */
  protected $allowedMimeTypes;
  /**
   *
   * @var <type>
   */
  protected $allowedDatastreams;
  /**
   *
   * @var <type>
   */
  protected $restrictedDatastreams;
  /**
   *
   * @var <type> 
   */
  protected $description;

  /**
   *
   * @param Fedora_Item $fedora_item
   */
  public function __construct(&$fedora_item, &$content_model) {
    $this->item = &$fedora_item;
    $pid = $content_model->pid;
    $content_model = new Fedora_Item($pid);
    $obj_viewer = $content_model->get_datastream_dissemination('OBJ_VIEWER');
    if (trim($obj_viewer) != '') {
      $this->objectViewDescription = new SimpleXMLElement($obj_viewer);
    }
    else {
      throw new Exception("OBJ_VIEWER datastream not found in the content model {$pid} of {$this->item->pid}.");
    }
  }

  /**
   *
   * @param string $dsid
   * @return array
   */
  public function canViewDatastream($dsid) {
    if ($this->isDatastreamIDRestricted($dsid)) {
      return FALSE;
    }
    else {
      if ($this->isDatastreamIDPermitted($dsid)) {
        return TRUE;
      }
      else {
        $mime_type = $this->item->datastreams[$dsid]['MIMEType'];
        $allowed_mime_types = $this->getAllowedMimeTypes();
        if (isset($allowed_mime_types[$mime_type]) && !$this->isIcon($dsid)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * 
   */
  private function isDatastreamIDRestricted($dsid) {
    $restricted_datastreams = $this->getRestrictedDatastreams();
    foreach ($restricted_datastreams as $pattern) {
      if ($pattern == $dsid || @preg_match($pattern, $dsid) === 1) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * 
   */
  private function isDatastreamIDPermitted($dsid) {
    $allowed_datastreams = $this->getAllowedDatastreams();
    foreach ($allowed_datastreams as $pattern) {
      if ($pattern == $dsid || @preg_match($pattern, $dsid) === 1) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   *
   * @return <type> 
   */
  private function isIcon($dsid) {
    return ($dsid == 'TN') || (preg_match('/-tn\.([^\/]*)$/i', $dsid) > 0) || $this->checkRelsIntForThumbnailExists($dsid);
  }

  /**
   *
   * @param <type> $dsid
   * @return boolean 
   */
  private function checkRelsIntForThumbnailExists($dsid) {
    $rels_int = $this->getRelsInt();
    if (isset($rels_int)) {
      $path = "//coal:hasThumbnail[@rdf:resource='info:fedora/{$this->item->pid}/{$dsid}']"; // coal Namespace is worry some.
      $results = $rels_int->xpath($path);
      if (is_array($results) && count($results) > 0) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Gets the RELS-INT datastream as a SimpleXMLElement.
   *
   * @return SimpleXMLElement
   */
  private function getRelsInt() {
    $rels_int = $this->item->get_datastream_dissemination("RELS-INT");
    if (trim($rels_int) != '') {
      return new SimpleXMLElement($rels_int);
    }
    return NULL;
  }

  /**
   *
   * @return array
   */
  public function getDescription() {
    if (empty($this->description)) {
      $items = $this->getDescriptionItems();
      $this->description = &$this->queryDescriptionItemPaths($items);
    }
    return $this->description;
  }

  /**
   *
   * @return array
   */
  private function getDescriptionItems() {
    $description_items = array();
    $foo = $this;
    $result = $this->objectViewDescription->xpath('/object_viewer/description/item');
    if (isset($result) && $result !== FALSE) {
      while (($item = array_shift($result)) != NULL) {
        $label = trim(sprintf("%s", $item->label));
        $path = trim(sprintf("%s", $item->path));
        $description_items[] = array($label, $path);
      }
    }
    return $description_items;
  }

  /**
   *
   * @param array $items
   * @return array
   */
  private function &queryDescriptionItemPaths(&$items) {
    $output = array();
    $result = $this->objectViewDescription->xpath('/object_viewer/description');
    $dsid = $result[0]['dsid'];
    $datastream = $this->item->get_datastream_dissemination($dsid);
    if (trim($datastream) != '') {
      $doc = new DOMDocument();
      $doc->loadXML($datastream);
      $root = &$doc->documentElement;
      $root->setAttributeNS(self::xmlns, 'xmlns', self::mods);
      $root->setAttributeNS(self::xmlns, 'xmlns:mods', self::mods);
      $root->setAttributeNS(self::xmlns, 'xmlns:xlink', self::xlink);
      $root->setAttributeNS(self::xmlns, 'xmlns:xsi', self::xsi);
      $xml = $doc->saveXML();
      $doc->loadXML($xml); // Sucha a terrible api...
      $xpath = new DOMXPath($doc);
      $xpath->registerNamespace('mods', 'http://www.loc.gov/standards/mods/v3');
      foreach ($items as $item) {
        list($label, $path) = $item;
        $results = $xpath->query($path);
        $count = $results->length;
        for ($i = 0; $i < $count; $i++) {
          $item = $results->item($i);
          switch (get_class($item)) {
            case 'DOMAttr':
              $value = trim($item->value);
              if ($value != '') {
                $output[$label] .= $this->makeLinks($value) . '<br/>';
              }
              break;
            case 'DOMElement':
              $value = trim($item->textContent);
              if ($value != '') {
                $output[$label] .= $this->makeLinks($value) . '<br/>';
              }
              break;
          }
        }
      }
    }
    return $output;
  }

  /**
   * This code snippit comes from the Remark Markup Library, Authored by Alan Tick. GNU GPL v3.
   * 
   * https://launchpad.net/remark
   * 
   * @param type $text
   * @return type 
   */
  private function makeLinks($text) {
    $SCHEMES = array('http', 'https', 'ftp', 'mailto', 'news',
      'gopher', 'nntp', 'telnet', 'wais', 'prospero', 'aim', 'webcal');
    // Note: fragment id is uchar | reserved, see rfc 1738 page 19
    // %% for % because of string formating
    // puncuation = ? , ; . : !
    // if punctuation is at the end, then don't include it
    $URL_FORMAT = '~(?<!\w)((?:' . implode('|', $SCHEMES) . '):' # protocol + :
        . '/*(?!/)(?:' # get any starting /'s
        . '[\w$\+\*@&=\-/]' # reserved | unreserved
        . '|%%[a-fA-F0-9]{2}' # escape
        . '|[\?\.:\(\),;!\'](?!(?:\s|$))' # punctuation
        . '|(?:(?<=[^/:]{2})#)' # fragment id
        . '){2,}' # at least two characters in the main url part
        . ')~';
    
    return preg_replace($URL_FORMAT, '<a href="\1">\1</a>', $text);
  }

  /**
   *
   * @param string $glue
   * @param array $values
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
   * @param array $values
   * @return array
   */
  private function getSimpleXMLArrayAsKeyedArrayWithTrueValues($values) {
    $output = array();
    if (isset($values) && $values !== FALSE) {
      while (($value = array_shift($values)) != NULL) {
        $value = trim(sprintf("%s", $value));
        if ($value != '') {
          $output[$value] = true;
        }
      }
    }
    return $output;
  }

  /**
   *
   * @return array
   */
  private function getAllowedMimeTypes() {
    if (empty($this->allowedMimeTypes)) {
      $result = $this->objectViewDescription->xpath('/object_viewer/allowed_datastreams/mime_types/mime_type');
      $this->allowedMimeTypes = $this->getSimpleXMLArrayAsKeyedArrayWithTrueValues($result);
    }
    return $this->allowedMimeTypes;
  }

  /**
   *
   * @return array
   */
  private function getAllowedDatastreams() {
    if (empty($this->allowedDatastreams)) {
      $result = $this->objectViewDescription->xpath('/object_viewer/allowed_datastreams/datastream_ids/dsid');
      $this->allowedDatastreams = $this->getSimpleXMLArray($result);
    }
    return $this->allowedDatastreams;
  }

  /**
   *
   * @return array
   */
  private function getRestrictedDatastreams() {
    if (empty($this->restrictedDatastreams)) {
      $result = $this->objectViewDescription->xpath('/object_viewer/restricted_datastreams/datastream_ids/dsid');
      $this->restrictedDatastreams = $this->getSimpleXMLArray($result);
    }
    return $this->restrictedDatastreams;
  }

  /**
   *
   * @param array $values
   * @return array
   */
  private function getSimpleXMLArray($values) {
    $output = array();
    if (isset($values) && $values !== FALSE) {
      while (($value = array_shift($values)) != NULL) {
        $output[] = (string) $value;
      }
    }
    return $output;
  }

}