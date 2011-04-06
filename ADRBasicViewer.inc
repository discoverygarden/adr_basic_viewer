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
    self::includeJSForFlowPlayer($pid);
    self::includeJSForPrettyPrint();
  }

  private static function includeJSForFlowPlayer($pid) {
    $path = drupal_get_path('module', 'adr_basic_viewer');
    $base_path = base_path();
    $full_path = $base_path . $path;
    drupal_add_js("$path/flowplayer/flowplayer-3.2.6.min.js");
    $js = <<<EOT
    function loadFlowPlayerAduio(dsid) {
      flowplayer("playerFlow", "$full_path/flowplayer/flowplayer-3.2.7.swf");
    }
    function loadFlowPlayer(dsid) {
      flowplayer("playerFlow", "$full_path/flowplayer/flowplayer-3.2.7.swf", {
        plugins: {
          controls: {
            fullscreen: false,
            autoHide: false
          }
        },
        clip: {
          autoPlay: false,
        }
      });
    }
EOT;
    drupal_add_js($js, 'inline');
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
        "924", "845", swfVersionStr, xiSwfUrlStr, flashvars, params, attributes);
      swfobject.createCSS("#playerFlexPaper", "display:block;text-align:left;");
   }
EOT;
    drupal_add_js($js, 'inline');
  }

  private static function includeJSForPrettyPrint() {
    $path = drupal_get_path('module', 'adr_basic_viewer');
    drupal_add_js("$path/pretty-print/prettify.js");
    drupal_add_css("$path/pretty-print/prettify.css");
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
    module_load_include('inc', 'adr_basic_viewer', 'ObjectViewDescription');
    module_load_include('inc', 'fedora_repository', 'ContentModel');
    $content_model = ContentModel::loadFromObject($this->pid);
    $obj_view = new ObjectViewDescription($this->item, $content_model);
    foreach ($this->item->datastreams as $dsid => $attr) {
      if ($obj_view->canViewDatastream($dsid)) {
        return $dsid;
      }
    }
  }

  /**
   *
   * @return string
   */
  private function getTextXmlViewer() {
    $filename = drupal_get_path('module', 'adr_basic_viewer') . '/xsl/PrettyPrint.xsl';
    $xsl = new DOMDocument();
    $xsl->load($filename);
    $doc = new DOMDocument();
    $doc->loadXML($this->item->get_datastream_dissemination($this->dsid));
    $proc = new XSLTProcessor();
    $proc->importStylesheet($xsl);
    $content = $proc->transformToXml($doc);
    $content = htmlentities($content);
    $output = '';
    foreach (preg_split("/(\r?\n)/", $content) as $line) {
      $output .= $line . '<br/>';
    }
    return "<code class='prettyprint xml' style='overflow: scroll; position: relative;'>$output</code>";
  }

  /**
   *
   * @return string
   */
  private function getDjatokaViewer() {
    $viewer_url = variable_get('fedora_base_url', '') . '/get/' . $this->pid . '/ilives:viewerSdef/getViewer?dsid=' . $this->dsid;
    return '<iframe src="' . $viewer_url . '" frameborder="0" style="width: 924px; height: 845px;">Errors: unable to load viewer</iframe>';
  }

  /**
   *
   * @return string
   */
  private function getVideoViewer() {
    $file_url = variable_get('fedora_base_url', '') . '/get/' . $this->pid . "/{$this->dsid}";
    return "<a href='$file_url' style='display:block;width:640px;height:480px;margin-left:auto;margin-right:auto;' id='playerFlow'/>";
  }

  private function getFlashImageViewer($imageList=null) {
    global $base_url;
    $path = drupal_get_path('module', 'Fedora_Repository');
    $fullPath = base_path() . $path;
    $swfURL = $base_url . base_path() . '/fedora/repository/' . $this->pid . '/WEB/web.jpg';
    $swfURL = 'image0=' . $swfURL . '&title0=IMAGE0';
    if (!empty($imageList)) {
      $swfURL = "";
      $counter = 0;
      foreach ($imageList as $image) {
        $swfURL .= 'image' . $counter . '=' . $base_url . base_path() . 'fedora/repository/' . $this->pid . '/' . $image . '&title' . $counter . '=IMAGE' . $counter++ . '&';
      }
      $swfURL = substr($swfURL, 0, strlen($swfURL) - 1);
    }
    $content = "";
    $content = '<div id="flashpictureviewer">
      <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,0,0" width="540" height="300" style="margin-left: auto; margin-right: auto;display:block;">
      <param name="movie" value="/sites/all/libraries/flash_image_viewer.swf" />
      <param name="quality" value="high" />
      <param name="wmode" value="transparent" />
      <param name="allowFullScreen" value="true" />
      <param name="allowScriptAccess" value="always" />
      <param name="FlashVars" value="' . $swfURL . '" / >
      <embed src="/sites/all/libraries/flash_image_viewer.swf"
      FlashVars="' . $swfURL . '"
      quality="high"
      type="application/x-shockwave-flash"
      WMODE="transparent"
      width="640"
      height="480"
      allowFullScreen="true"
      allowScriptAccess="always"
      pluginspage="http://www.macromedia.com/go/getflashplayer" />
      </object></div>';
    return $content;
  }

  /**
   *
   * @return string
   */
  public function getViewer() {
    switch ($this->mimeType) {
      case 'application/rdf+xml':
      case 'text/xml':
        return $this->getTextXmlViewer();
      case 'video/x-flv':
        return "<div id='playerFLV' style='margin-left:auto; margin-right: auto;'></div>";
      case 'audio/mpeg':
      case 'audio/x-mpeg':
      case 'video/mpeg':
      case 'video/mp4':
        return $this->getVideoViewer();
      case 'application/pdf':
        return '<div id="playerFlexPaper"></div>';
      case 'image/jpeg':
      case 'image/jp2':
      case 'image/tiff':
      case 'image/png':
        return $this->getDjatokaViewer();
      default:
        return '<div>There is no viewer for the selected file type, or not file is selected.</div>';
    }
  }

}

