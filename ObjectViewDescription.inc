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
  ///////////////////////////
  /**
   * If true don't show thumbnail datastreams in the files panel.
   * 
   * @var boolean
   */
  protected $hideThumbnails;
  /**
   * If true don't show derived datastreams in the files panel.
   * 
   * @var boolean
   */
  protected $hideDerived;
  /**
   * A list of datastreams defined as thumbnails in the rels-int.
   * 
   * @var array
   */
  protected $thumbnails;
  /**
   * A list of datastreams defined as derived in the rels-int.
   * 
   * @var array
   */
  protected $derived;
  /**
   * A list of all viewable MIME types.
   * 
   * @var array
   */
  protected $viewableMimeTypes;
  /**
   * A list of all hidden MIME types.
   * 
   * @var array
   */
  protected $hiddenMimeTypes;
  /**
   * A list of all downloadable MIME types.
   * 
   * @var array
   */
  protected $downloadableMimeTypes;
  /**
   * A list of all downloadable MIME types.
   * 
   * @var array
   */
  protected $nonDownloadableMimeTypes;
  /**
   * A list of viewable datastreams.
   *  
   * @var array
   */
  protected $viewableDatastreams;
  /**
   * A list of hidden datastreams.
   *  
   * @var array
   */
  protected $hiddenDatastreams;
  /**
   * A list of all downloadable datastreams.
   * 
   * @var array
   */
  protected $downloadableDatastreams;
  /**
   * A list of all non-downloadable datastreams.
   * 
   * @var array
   */
  protected $nonDownloadableDatastreams;

  /**
   *
   * @param Fedora_Item $fedora_item
   */
  public function __construct(&$fedora_item, &$content_model) {
    $this->item = &$fedora_item;
    $xml = $this->getObjViewerDefinition($content_model->pid);
    $this->processObjViewerDefinition($xml);
    $xml = $this->getRelsInt();
    $this->processRelsInt($xml);
  }

  /**
   * Gets the RELS-INT datastream as a SimpleXMLElement.
   *
   * @return SimpleXMLElement
   */
  private function getRelsInt() {
    $rels_int = $this->item->get_datastream_dissemination("RELS-INT");
    return (trim($rels_int) != '') ? $rels_int : NULL;
  }

  /**
   * Get the description that defines the Obj Viewers properties.
   * @param string $pid 
   */
  private function getObjViewerDefinition($pid) {
    $content_model = new Fedora_Item($pid);
    /*
      $xml = $content_model->get_datastream_dissemination('OBJ_VIEWER');
      if (trim($xml) == '') {
      throw new Exception("OBJ_VIEWER datastream not found in the content model {$pid} of {$this->item->pid}.");
      }
      return $xml; */
    // Hacktastic.
    $path = drupal_get_path('module', 'adr_basic_viewer');
    $obj_viewer = new DOMDocument();
    $obj_viewer->load("$path/obj_viewer.xml");
    return $obj_viewer->saveXML();
  }

  /**
   * Intialize this instances members based on information stored within the obj_viewer definition.
   * 
   * @param string $xml
   */
  private function processObjViewerDefinition($xml) {
    $object_viewer = new SimpleXMLElement($xml);
    $this->viewableMimeTypes = $this->getValuesFromXPath($object_viewer, '//mime_type[@view = \'true\']');
    $this->hiddenMimeTypes = $this->getValuesFromXPath($object_viewer, '//mime_type[@view = \'false\']');
    $this->downloadableMimeTypes = $this->getValuesFromXPath($object_viewer, '//mime_type[@download = \'true\']');
    $this->nonDownloadableMimeTypes = $this->getValuesFromXPath($object_viewer, '//mime_type[@download = \'false\']');
    $this->viewableDatastreams = $this->getValuesFromXPath($object_viewer, '//dsid[@view = \'true\']');
    $this->hiddenDatastreams = $this->getValuesFromXPath($object_viewer, '//dsid[@view = \'false\']');
    $this->downloadableDatastreams = $this->getValuesFromXPath($object_viewer, '//dsid[@download = \'true\']');
    $this->nonDownloadableDatastreams = $this->getValuesFromXPath($object_viewer, '//dsid[@download = \'false\']');
    $this->hideDerived = $object_viewer->files['hide_derived'] == 'true' ? TRUE : FALSE;
    $this->hideThumbnails = $object_viewer->files['hide_thumbnails'] == 'true' ? TRUE : FALSE;
  }

  /**
   * 
   * 
   * @param SimpleXMLElement $object_viewer
   * @param string $path
   * @return array
   */
  private function getValuesFromXPath(SimpleXMLElement $element, $path) {
    $output = array();
    $results = $element->xpath($path);
    foreach ($results as $result) {
      $output[] = (string) $result;
    }
    return $output;
  }

  /**
   * Initialize instance members based on information stored within this objects rels-int.
   * 
   * @param string $xml 
   */
  private function processRelsInt($xml) {
    $rels_int = new SimpleXMLElement($xml);
    // Thumbnails
    $this->thumbnails = array();
    $results = $rels_int->xpath('//*[local-name()=\'hasThumbnail\']/@rdf:resource');
    foreach ($results as $result) {
      $matches = array();
      if (preg_match('/\/([^\/]*)$/', (string) $result['resource'], $matches) == 1) {
        $this->thumbnails[] = $matches[1];
      }
    }
    // Derived
    $this->derived = array();
    $results = $rels_int->xpath('//rdf:Description/*/@rdf:resource');
    foreach ($results as $result) {
      $matches = array();
      if (preg_match('/\/([^\/]*)$/', (string) $result['resource'], $matches) == 1) {
        $this->derived[] = $matches[1];
      }
    }
  }

  /**
   * Search for...
   * 
   * @param string $needle
   * @param array $haystack
   * @return boolean
   */
  public function searchPatternArray($needle, array &$haystack) {
    foreach ($haystack as $pattern) {
      if ($pattern == $needle || @preg_match($pattern, $needle) === 1) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   *
   * @param string $dsid
   * @return boolean 
   */
  public function isDatastreamViewable($dsid) {
    // Check Derived
    if ($this->hideDerived && $this->isDerived($dsid)) {
      return FALSE;
    }
    // Check Thumbnail
    else if ($this->hideThumbnails && $this->isThumbnail($dsid)) {
      return FALSE;
    }
    // Check if viewable
    else if ($this->searchPatternArray($dsid, $this->viewableDatastreams)) {
      return TRUE;
    }
    // Check if explicitly hidden.
    else if ($this->searchPatternArray($dsid, $this->hiddenDatastreams)) {
      return FALSE;
    }
    // Check if MIME type of the given datastream is viewable
    else {
      return $this->isMimeTypeViewable($this->item->datastreams[$dsid]['MIMEType']);
    }
  }

  /**
   *
   * @param string $mime_type
   * @return boolean 
   */
  public function isMimeTypeViewable($mime_type) {
    if (array_search($mime_type, $this->viewableMimeTypes) !== FALSE) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * @param type $dsid 
   * @return boolean
   */
  public function isDatastreamDownloadable($dsid) {
    if ($this->isDatastreamViewable($dsid)) {
      if ($this->searchPatternArray($dsid, $this->downloadableDatastreams)) {
        return TRUE;
      }
      else if ($this->searchPatternArray($dsid, $this->nonDownloadableDatastreams)) {
        return FALSE;
      }
      else {
        return $this->isMimeTypeDownloadable($this->item->datastreams[$dsid]['MIMEType']);
      }
    }
    return FALSE;
  }

  /**
   *
   * @param string $mime_type 
   * @return boolean
   */
  public function isMimeTypeDownloadable($mime_type) {
    if (array_search($mime_type, $this->downloadableMimeTypes) !== FALSE) {
      return TRUE;
    }
    else if (array_search($mime_type, $this->nonDownloadableDatastreams) !== FALSE) {
      return FALSE;
    }
    return FALSE;
  }

  /**
   * Determines if the datastream is a thumbnail.
   * 
   * @param string $dsid 
   * @return boolean
   */
  public function isThumbnail($dsid) {
    if (array_search($dsid, $this->thumbnails) !== FALSE) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * @param string $dsid 
   * @return boolean
   */
  public function isDerived($dsid) {
    if (array_search($dsid, $this->derived) !== FALSE) {
      return TRUE;
    }
    return FALSE;
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
    /*$foo = $this;
    $result = $this->objectViewDescription->xpath('/object_viewer/description/item');
    if (isset($result) && $result !== FALSE) {
      while (($item = array_shift($result)) != NULL) {
        $label = trim(sprintf("%s", $item->label));
        $path = trim(sprintf("%s", $item->path));
        $description_items[] = array($label, $path);
      }
    }*/
    return $description_items;
  }

  /**
   *
   * @param array $items
   * @return array
   */
  private function &queryDescriptionItemPaths(&$items) {
    $output = array();
    /*
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
    }*/
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