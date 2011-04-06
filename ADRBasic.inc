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
   * Create a view for describing/managing an object.
   *
   * @param string $pid
   */
  public function __construct($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->pid = $pid;
  }
  
  /**
   * 
   */
  private function includeRequiredScripts() {
    $path = drupal_get_path('module', 'adr_basic_viewer');
    module_load_include('module', 'ext', 'ext');
    drupal_add_css("$path/stylesheets/Clear.css", 'theme', 'all');
    ext_load_library();
    drupal_add_css("$path/stylesheets/ADRBasic.css", 'theme', 'all');
    module_load_include('inc', 'adr_basic_viewer', 'ADRBasicViewer');
    ADRBasicViewer::includeRequiredJSForViewers($this->pid);

    $data_stores = array(
      'Datastreams.js',
      'ObjectProperties.js',
      'OverviewDatastreams.js',
      'ControlGroups.js',
      'MIMETypes.js',
      'State.js',
    );

    $custom_components = array(
      'FileUploadField.ui.js',
      'FileUploadField.js',
    );

    $main_viewer = array(
      'ADRBasicViewer.ui.js',
      'ADRBasicViewer.js',
      'OverviewPanel.ui.js',
      'OverviewPanel.js',
      'DescriptionPanel.ui.js',
      'DescriptionPanel.js',
      'Files.ui.js',
      'Files.js',
      'ViewerPanel.ui.js',
      'ViewerPanel.js',
    );

    $manage_panel = array(
      'ManagePanel.ui.js',
      'ManagePanel.js',
      'EditObjectPanel.ui.js',
      'EditObjectPanel.js',
      'EditFilesPanel.ui.js',
      'EditFilesPanel.js',
    );

    $manage_windows = array(
      'AddFileWindow.ui.js',
      'AddFileWindow.js',
      //'ReplaceFileWindow.ui.js',
      //'ReplaceFileWindow.js',
      'EditObjectWindow.ui.js',
      'EditObjectWindow.js',
    );

    drupal_add_js("$path/js/IncludeFirst.js");
    $this->includeScripts($data_stores, $path);
    $this->includeScripts($custom_components, $path);
    $this->includeScripts($main_viewer, $path);
    if(user_access('purge objects and datastreams') && user_access('add fedora datastreams') && user_access('edit fedora meta data')) {
      $this->includeScripts($manage_panel, $path);
      $this->includeScripts($manage_windows, $path);
    }
    drupal_add_js("$path/js/ADRBasic.js");
  }

  /**
   *
   * @param array $scripts
   * @param string $path
   */
  private function includeScripts($scripts, $path) {
    foreach ($scripts as $file) {
      drupal_add_js("$path/js/$file");
    }
  }

  /**
   *
   */
  public function render() {
    $this->includeRequiredScripts(); // Javascript all the way!
  }

}