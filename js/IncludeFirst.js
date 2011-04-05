/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Ext.ns("ADRBasic");
ADRBasic.pid = window.location.pathname.split('/')[3];
ADRBasic.viewer = {
    pid: ADRBasic.pid,
    label: '',
    dsid: null,
    setDatastream: function(label, dsid) {
        if(label != null) {
            this.label = label;
        }
        if(dsid != null) {
            this.dsid = dsid;
        }
    },
    setTitle: function() {
        Ext.getCmp('adr-viewer-panel').setTitle(this.label + " (" + this.dsid + ")");
    },
    loadViewer: function() {
        var viewer = Ext.getCmp('adr-viewer');
        if(viewer.rendered) {
            var loadOptions = viewer.autoLoad;
            loadOptions.params.dsid = this.dsid;
            viewer.load(loadOptions);
        }
        else {
            viewer.autoLoad.params.dsid = this.dsid;
        }
    },
    showViewerTab: function() {
        Ext.getCmp('adr-viewer-tab').show();
    },
    show: function (label, dsid) {
        this.setDatastream(label, dsid);
        this.setTitle();
        this.loadViewer();
        this.showViewerTab();
    },
    init: function () {
        var store = Ext.StoreMgr.lookup('OverviewDatastreams');
        store.addListener('load', function(store, records) {
            var viewerTab = Ext.getCmp('adr-viewer-tab');
            if(viewerTab.title == "Viewer") { // View has not been set.
                var record = records[0];
                var label = record.get('label');
                var dsid = record.get('dsid');
                ADRBasic.viewer.setDatastream(label, dsid);
                ADRBasic.viewer.setTitle();
            }
        });
        var record = store.getAt(0);
        if(record) {
            var label = record.get('label');
            var dsid = record.get('dsid');
            ADRBasic.viewer.setDatastream(label, dsid);
            ADRBasic.viewer.setTitle();
        }
    }
};