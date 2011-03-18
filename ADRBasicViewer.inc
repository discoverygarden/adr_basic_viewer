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
    self::includeJSForFlexPaper($pid);
  }

  private static function includeJSForFlash($pid) {
    $path = drupal_get_path('module', 'Fedora_Repository');
    $base_path = base_path();
    $full_path = $base_path . $path;
    drupal_add_js("$path/js/swfobject.js");
    $js = <<<EOT
    function loadFLVPlayer(dsid) {
      var basePath = window.location.protocol + '//' + window.location.hostname;
      var pid = window.location.pathname.split('/')[3];
      var s1 = new SWFObject( '$full_path/flash/flvplayer.swf', 'single', '320', '240', '7');
      s1.addParam('allowfullscreen', 'TRUE');
      s1.addVariable('file', '{$base_path}fedora/repository/$pid/' + dsid);
      s1.write('playerFLV');
    }
EOT;
    drupal_add_js($js, 'inline');
  }

  private static function includeJSForFlexPaper($pid) {
    global $base_url;
    drupal_add_js("sites/all/libraries/flexpaper/js/flexpaper_flash.js");
    drupal_add_js("sites/all/libraries/swfobject/swfobject.js");
    $js = <<<EOT
    function loadFlexPlayer(dsid) {
      var dsid = dsid.replace('pdf', 'swf');
      var swfURL = '$base_url/fedora/repository/{$pid}/' + dsid;
      var swfVersionStr = "10.0.0";
      var xiSwfUrlStr = "/sites/all/libraries/flexpaper/js/swfobject/expressInstall.swf";
      var flashvars = {
        SwfFile : escape(swfURL),
        Scale : 0.6,
        ZoomTransition : "easeOut",
				ZoomTime : 0.5,
        ZoomInterval : 0.1,
  			FitPageOnLoad : false,
        FitWidthOnLoad : true,
  			PrintEnabled : true,
    		FullScreenAsMaxWindow : false,
  			ProgressiveLoading : true,
  			PrintToolsVisible : true,
  			ViewModeToolsVisible : true,
  			ZoomToolsVisible : true,
  			FullScreenVisible : true,
  			NavToolsVisible : true,
  			CursorToolsVisible : true,
  			SearchToolsVisible : true,
  			localeChain: "en_US"
      };
      var params = {}
      params.quality = "high";
      params.bgcolor = "#ffffff";
      params.allowscriptaccess = "sameDomain";
      params.allowfullscreen = "true";
      var attributes = {};
      attributes.id = "FlexPaperViewer";
      attributes.name = "FlexPaperViewer";
      swfobject.embedSWF(
        "$base_url/sites/all/libraries/flexpaper/FlexPaperViewer.swf", "playerFlexPaper",
        "960", "800", swfVersionStr, xiSwfUrlStr, flashvars, params, attributes);
      swfobject.createCSS("#playerFlexPaper", "display:block;text-align:left;");
   }
EOT;
    drupal_add_js($js, 'inline');
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
    module_load_include('inc', 'adr_basic_viewer', 'ADRBasic');
    foreach ($this->item->datastreams as $dsid => $attr) {
      if (ADRBasic::canShowDatastreamInOverview($dsid, $attr)) {
        return $dsid;
      }
    }
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
      case 'video/x-flv':
        return "<div id='playerFLV' style='margin-left:auto; margin-right: auto;'></div>";
      case 'application/pdf':
        //return "<iframe src='http://docs.google.com/viewer?url=$base_url/fedora/repository/{$this->pid}/{$this->dsid}/&embedded=TRUE' style='width:960px; height:953px;' frameborder='0'></iframe>";
        return '<div id="playerFlexPaper"></div>';
      case 'image/jpeg':
        //$viewer_url = variable_get('fedora_base_url', '') . '/get/' . $this->pid . '/ilives:viewerSdef/getViewer';
        //return '<iframe src="' . $viewer_url . '" frameborder="0" style="width: 100%; height: 400px;">Errors: unable to load viewer</iframe>';
        return "<a href='$base_url/fedora/repository/{$this->pid}/{$this->dsid}/'><img src='$base_url/fedora/repository/{$this->pid}/{$this->dsid}' style='display:block; margin-left:auto; margin-right: auto;'/></a>";
      default:
        return '<div>There is no viewer for the selected file type.</div>';
    }
  }

}

