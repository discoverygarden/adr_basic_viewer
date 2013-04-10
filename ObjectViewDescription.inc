<?php

// $Id$

/**
 * @file
 *
 */
class ObjectViewDescription {

  /**
   *
   * @var Fedora_Item
   */
  protected $item;

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
   * A collection of labels and values for use in the description panel.
   *
   * @var array
   */
  protected $description;

  /**
   * Create the object view description.
   *
   * @param Fedora_Item $fedora_item
   * @param ContentModel $content_model
   */
  public function __construct(Fedora_Item &$fedora_item, ContentModel &$content_model) {
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
    if(isset($this->item->datastreams['RELS-INT'])) {
      $rels_int = $this->item->get_datastream_dissemination("RELS-INT");
      return (trim($rels_int) != '') ? $rels_int : NULL;
    }
    else {
      return NULL;
    }
  }

  /**
   * Get the description that defines the Obj Viewers properties.
   * @param string $pid
   */
  private function getObjViewerDefinition($pid) {
    $content_model = new Fedora_Item($pid);
    $xml = trim($content_model->get_datastream_dissemination('OBJ_VIEWER'));
    if ($xml == '') {
      throw new Exception("OBJ_VIEWER datastream not found in the content model {$pid} of {$this->item->pid}.");
    }
    return $xml;
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
    $this->initializeDescription($object_viewer->description);
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
   *
   * @param SimpleXMLElement $description
   */
  private function initializeDescription(SimpleXMLElement $description) {
    $this->description = array();
    $metadata = $this->getDescriptionMetadata($description);
    if (isset($metadata)) {
      $items = $description->xpath('//item');
      foreach ($items as $item) {
        list($label, $value) = $this->getItemLabelAndValue($item, $metadata);
        if (isset($value)) {
          $this->description[$label] = $value;
        }
      }
    }
  }

  /**
   *
   * @param SimpleXMLElement $description
   * @return SimpleXMLElement
   */
  private function getDescriptionMetadata(SimpleXMLElement $description) {
    $metadata_dsid = (string) $description['dsid'];
    $metadata_xml = trim($this->item->get_datastream_dissemination($metadata_dsid));
    $metadata_xml = ($metadata_xml != '') ? $metadata_xml : NULL;
    if (isset($metadata_xml)) {
      $simple = new SimpleXMLElement($metadata_xml);
      $simple->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3'); // Hack!!
      return $simple;
    }
    return NULL;
  }

  /**
   *
   * @param SimpleXMLElement $item
   * @param SimpleXMLElement $metadata
   * @return type
   */
  private function getItemLabelAndValue(SimpleXMLElement $item, SimpleXMLElement $metadata) {
    $label = (string) $item->label;
    $value = $this->getItemValue($item, $metadata);
    return array($label, $value);
  }

  /**
   *
   * @param SimpleXMLElement $item
   * @param SimpleXMLElement $metadata
   * this is basically the worst code I've read in awhile. I have no idea what foo and bar do.
   * fixed a big that was returning bad data, but i'm not sure if i fixed the root cause.
   * @return type
   */
  private function getItemValue(SimpleXMLElement $item, SimpleXMLElement $metadata) {
    global $base_url;
    module_load_include('inc', 'islandora_solr_search', 'IslandoraSolrResults');
    module_load_include('php', 'islandora_solr_search', 'Solr/Service');
    $value = '';
    if (isset($item->value)) { // Only One Path
      $path = (string) $item->value->path;
      $field = isset($item->value->path->attributes()->field) ?
          (string) $item->value->path->attributes()->field :
          NULL;
      $results = $metadata->xpath($path);
      if ($results !== FALSE) {
        foreach ($results as $result) {
          $result = trim((string) $result);
          if ($result != '') {
            if ($this->isLink($result)) {
              $value .= $this->makeLinks($result) . '<br/>';
            }
            else if (isset($field)) {
              $search = trim($field) . ':"' . $this->lesser_escape(trim($result)) . '"';
              $search = htmlspecialchars(drupal_urlencode($search), ENT_QUOTES, 'utf-8', false);
              $search = str_replace('/', '~slsh~', $search); //replace the slash so url doesn't break
              $link = "<a href='$base_url/islandora/solr/search/$search/-'>$result</a><br/>";
              $value .= $link;
            }
            else {
              $value .= $result . '</br>';
            }
          }
        }
      }
      return ($value != '') ? $value : NULL;
    }
    else if (isset($item->values)) { // Multiple Paths
      $values = $item->values;
      // Determine Paths
      $foo = array();
      foreach ($values->children() as $path) {
        $field = isset($path->attributes()->field) ?
            (string) $path->attributes()->field :
            NULL;
        $foo[] = array((string) $path, $field);
      }
      // Determine parent context
      $values_found = FALSE;
      $context = $values['context'];
      $seperator = isset($values['seperator']) ? $values['seperator'] : $values['separator'];
      $parents = $metadata->xpath($context);
      if (is_array($parents)) {
        foreach ($parents as $parent) {
          $line_populated = FALSE;
          foreach ($foo as $bar) {
            list($path, $field) = $bar;
            $results = $parent->xpath($path);
            if ($results) {
            foreach ($results as $result) {
              $result = trim((string) $result);
              if ($result != '') {
                if ($this->isLink($result)) {
                  $value .= $this->makeLinks((!$values_found) ? $result : $seperator . $result);
                }
                else if (isset($field)) {
                  $search = trim($field) . ':"' . $this->lesser_escape(trim($result)) . '"';
                  $search = htmlspecialchars(drupal_urlencode($search), ENT_QUOTES, 'utf-8', false);
                  $search = str_replace('/', '~slsh~', $search); //replace the slash so url doesn't break
                  $link = "<a href='$base_url/islandora/solr/search/$search/-'>$result</a>";
                  $value .= $link;
                }
                else {
                  $value .= $result;
                }
                $values_found = TRUE;
                $line_populated = TRUE;
              }
            }
            }
          }
          if ($line_populated) {
            $value .= '<br/>';
          }
        }
      }
      return ($values_found) ? $value : NULL;
    }
    return NULL;
  }

  /**
   * Initialize instance members based on information stored within this objects rels-int.
   *
   * @param string $xml
   */
  private function processRelsInt($xml) {
    $this->thumbnails = array();
    $this->derived = array();
    if ($xml) {
      $relsInt = new SimpleXMLElement($xml);

      // Make a list of all the thumbnails (no association with DSIDs are necessary as
      // the only use of this list is to hide thumbnails when that option is selected).
      $results = $relsInt->xpath('//*[local-name()=\'hasThumbnail\']/@rdf:resource');
      if ($results !== FALSE) {
        foreach ($results as $result) {
          $matches = array();
          if (preg_match('/\/([^\/]*)$/', (string) $result['resource'], $matches) == 1) {
            $this->thumbnails[] = $matches[1];
          }
        }
      }

      // check each description block to see if there are derived types.  The array of
      // derived types is used to map the DSIDs of viewable types with the DSIDs that are
      // recognized but for which there is no viewer installed)
      $rdfDescriptions = $relsInt->xpath('//rdf:Description');
      foreach($rdfDescriptions as $currDescription) {

        // see if there is an "about" tag that represents the DSID of the "base" datastream
        // and if there is save the DSID name
        $aboutAttribute = $currDescription->xpath('./@rdf:about');
        $aboutNameMatches = array();
        if (count($aboutAttribute) > 0 &&
            preg_match('/\/([^\/]*)$/', (string) $aboutAttribute[0]['about'], $aboutNameMatches) == 1) {
          $baseDsid = $aboutNameMatches[1];

          // if there is a base DSID then check for all the resources that are NOT thumbnails
          // and store this list of resources in an array that represents the derived DSIDs
          // for the base DSID.
          $rdfResources = $currDescription->xpath('*/@rdf:resource');
          $dsidDerived = array();
          foreach($rdfResources as $currResource) {
            $hasRelationshipMatches = array();
            if (preg_match('/\/([^\/]*)$/', (string) $currResource['resource'], $hasRelationshipMatches) == 1 &&
                !in_array($hasRelationshipMatches[1], $this->thumbnails)) {
              if(stripos($hasRelationshipMatches[1], 'swf') === FALSE) {
                $dsidDerived[] = $hasRelationshipMatches[1];
              }

            }
          }

          // finally, if there are derived types for a base type, add them to the derived array
          // as an array of derived DSIDs assiciated with the base DSID.
          if (count($dsidDerived) > 0) {
            $this->derived[$baseDsid] = $dsidDerived;
          }
        }
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
    global $user;
    // load xacml stuff
    module_load_include('inc', 'adr_basic_viewer', 'ADRBasicViewer');
    module_load_include('inc', 'islandora_xacml_api', 'Xacml');
    module_load_include('inc', 'islandora_xacml_api', 'XacmlException');

    try {
      $xacml = Xacml::constructFromPid($this->item->pid);

      // if the user does not have permission to view this DSID then it is
      // still possible that there is a derived datastream that the user is
      // permitted to view.  So ... check the derived streams to see if there
      // is one that the user can view.
      if(!$xacml->datastreamRule->hasPermission($user->name, $user->roles, $this->item->datastreams[$dsid]['MIMEType'], $dsid)){

        $viewable = $this->getViewableDSID($dsid);

        if( $viewable && $xacml->datastreamRule->hasPermission($user->name, $user->roles, $this->item->datastreams[$viewable]['MIMEType'], $viewable)) {
          return TRUE;
        }
        return FALSE;
      }
    }
    catch (XacmlException $e) {
      return FALSE;
    }


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
    // Check if MIME type of the given datastream is viewable.  This does NOT
    // mean that there is a viewer for this MIMEtype, just that we think that
    // there should be one somewhere because we _support_ this MIMEtype.
    else {
      $viewable = $this->getViewableDSID($dsid);
      $viewable_mime = $this->item->datastreams[$viewable]['MIMEType'];
      $this_mime = $this->item->datastreams[$dsid]['MIMEType'];
      return $this->isMimeTypeViewable($this_mime, $viewable_mime);
    }
  }

  /**
   *
   * Returns a viewable DSID based on whether the object has derivbed types that are
   * viewable and have a viewer or if the input DSID has a viewer (in which
   * case the input DSID is returned).
   *
   * Preference is given to the derived types first as they are usually created as
   * viewable alterntives (either because they are using a differnt format or they
   * are optimized for viewing in the same format--often made smaller) than the original
   * datastream.
   *
   * @param string $dsid
   * @return string
   */
  public function getViewableDSID($dsid) {
    global $user;

    module_load_include('inc', 'adr_basic_viewer', 'ADRBasicViewer');

    $viewable_dsid = NULL;

    try {
      module_load_include('inc', 'islandora_xacml_api', 'Xacml');
      module_load_include('inc', 'islandora_xacml_api', 'XacmlException');

      $xacml = Xacml::constructFromPid($this->item->pid);

      // check to see if there are derived streams for a DSID and if they can be displayed
      // by existing viewers AND also that the user has permission to view that datastream.
      if (array_key_exists($dsid, $this->derived)) {
        foreach ($this->derived[$dsid] as $derived_dsid) {
          if (ADRBasicViewer::allowedMimeType($this->item->datastreams[$derived_dsid]['MIMEType']) &&
              $xacml->datastreamRule->hasPermission($user->name, $user->roles, $this->item->datastreams[$derived_dsid]['MIMEType'], $derived_dsid)){
            $viewable_dsid = $derived_dsid;
            break;
          }
        }
      }

      // If no viewable dartastream has been found then test to see if the input DSID
      // both has a viewer and that the user has permission to view the DSID and MIME type.
      if (is_null($viewable_dsid) &&
          ADRBasicViewer::allowedMimeType($this->item->datastreams[$dsid]['MIMEType']) &&
          $xacml->datastreamRule->hasPermission($user->name, $user->roles, $this->item->datastreams[$dsid]['MIMEType'], $dsid)) {
        $viewable_dsid = $dsid;
      }
    }

    // default action is to return a NULL DSID.
    catch (XacmlException $e) {
    }

    return $viewable_dsid;
  }

  /**
   *
   * @param string $mime_type
   * @return boolean
   */
  public function isMimeTypeViewable($mime_type, $viewable_mime) {
    if (array_search($mime_type, $this->viewableMimeTypes) !== FALSE) {

      if(ADRBasicViewer::allowedMimeType($viewable_mime)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   *
   * @param type $dsid
   * @return boolean
   */
  public function isDatastreamDownloadable($dsid) {
    global $user;

    //if ($this->isDatastreamViewable($dsid)) {
      try{
        $xacml = Xacml::constructFromPid($this->item->pid);
        if(!$xacml->datastreamRule->hasPermission($user->name, $user->roles, $this->item->datastreams[$dsid]['MIMEType'], $dsid)) {
          return FALSE;
        }
      }
      catch (XacmlException $e) {
        return FALSE;
      }
      // Check Derived
      if ($this->hideDerived && $this->isDerived($dsid)) {
        return FALSE;
      }
      // Check Thumbnail
      else if ($this->hideThumbnails && $this->isThumbnail($dsid)) {
        return FALSE;
      }
      else if ($this->searchPatternArray($dsid, $this->downloadableDatastreams)) {
        return TRUE;
      }
      else if ($this->searchPatternArray($dsid, $this->nonDownloadableDatastreams)) {
        return FALSE;
      }
      else {
        return $this->isMimeTypeDownloadable($this->item->datastreams[$dsid]['MIMEType']);
      }
    //}
    //return FALSE;
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
    foreach($this->derived as $currDerivedArray) {
      if (array_search($dsid, $currDerivedArray) !== FALSE) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   *
   * @param string $dsid
   * @return array
   */
  public function isViewableAndDownloadable($dsid) {
    return array($this->isDatastreamViewable($dsid), $this->isDatastreamDownloadable($dsid));
  }

  /**
   *
   * @return array
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * This code snippit comes from the Remark Markup Library, Authored by Alan Tick. GNU GPL v3.
   *
   * https://launchpad.net/remark
   *
   * @param type $text
   * @return type
   */
  private function isLink($text) {
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

    return preg_match($URL_FORMAT, $text) > 0;
  }

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

  public function lesser_escape($value) {
    //list taken from http://lucene.apache.org/java/docs/queryparsersyntax.html#Escaping%20Special%20Characters
    //Zac removed the quotation mark and whitespace from this list.
    $pattern = '/(\+|-|&&|\|\||!|\(|\)|\{|}|\[|]|\^|~|\*|\?|:|\\\)/';
    $replace = '\\\$1';

    return preg_replace($pattern, $replace, $value);
  }

}

/**
 * Code from Paul.
 *
 * if(!empty($value)){
   if($fieldName == 'Author' || $fieldName == 'Topic' || $fieldName == "Publisher" || $fieldName == "Corporate Name(s)" || $fieldName == "Subject (Personal)" ||
           $fieldName == "Subject (Time)" || $fieldName == "Geography" || $fieldName == "Form" || $fieldName == "Place Of Publication" ||
           $fieldName == "Series" || $fieldName == "Language" || $fieldName == "Date Created" ||
           $fieldName == "Date Digitized") {
       foreach($value as $va){
       //$html .= '<a href = "'.$base_url.'/islandora/solr/search/'.$solr_search->escape($fieldName).drupal_urlencode(':"'.$va.'"').'">'.$va .'</a><br />';
       $html .= '<a href = "'.$base_url.'/islandora/solr/search/'.drupal_urlencode($solr_search->escape($fieldName)).drupal_urlencode(':"'.Apache_Solr_Service::escapePhrase($va).'"').'">'.$va .'</a><br />';

       }
   }   else {
   foreach( $value as $va) {
     $html .= $va .'<br />';
   }
 */
