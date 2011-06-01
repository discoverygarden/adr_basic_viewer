/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Ext.ns("ADRBasic");
ADRBasic.pid = decodeURIComponent(window.location.pathname.split('/')[3]);

ADRBasic.overview = {
    resize: function() {
    }
};

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
    isSet: function() {
        return this.dsid != null;
    },
    setTitle: function() {
        var store = Ext.StoreMgr.lookup('OverviewDatastreams');
        var total = store.getTotalCount();
        var title = 'Files (Total: ' + total + ')';
        Ext.getCmp('adr-overview-files').setTitle(title);
        Ext.getCmp('adr-viewer-files').setTitle(title);
        Ext.getCmp('adr-viewer').setTitle(this.dsid + " (" + this.label + ")");
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
                ADRBasic.viewer.loadViewer();
            }
        });
        var record = store.getAt(0);
        if(record) {
            var label = record.get('label');
            var dsid = record.get('dsid');
            ADRBasic.viewer.setDatastream(label, dsid);
            ADRBasic.viewer.setTitle();
            ADRBasic.viewer.loadViewer();
        }
        this.autoAttachFlash();
        var viewer = Ext.getCmp('adr-viewer');
        viewer.addListener('resize', this.resize);
        
    },
    resize: function(panel, adjWidth, adjHeight, rawWidth, rawHeight) {
        var flexPaper = Ext.get('FlexPaperViewer');
        if(flexPaper) {
            flexPaper.width = rawWidth;
            flexPaper.height = rawHeight;
        }
    },
    shouldLoadFlexPaper: function() {
        var flexPaper = Ext.get('playerFlexPaper');
        if(flexPaper && !flexPaper.hasClass('loaded-flex-player')) {
            return true;
        }
        return false;
    },
    loadFlexPaper: function(dsid) {
        var flexPaper = Ext.get('playerFlexPaper');
        flexPaper.addClass('loaded-flex-player');
        loadFlexPlayer(dsid);
    },
    shouldLoadFlowPlayer: function() {
        var flowPlayer = Ext.get('playerFlow');
        if(flowPlayer && !flowPlayer.hasClass('loaded-flow-player')) {
            return true;
        }
        return false;
    },
    loadFlowPlayer: function(dsid) {
        var flowPlayer = Ext.get('playerFlow');
        flowPlayer.addClass('loaded-flow-player');
        loadFlowPlayer(dsid);
    },
    shouldLoadPrettyPrinter: function() {
        var el = $('.prettyprint');
        if(el && !el.hasClass('.loaded-pretty-print')) {
            return true;
        }
        return false;
    },
    loadPrettyPrinter: function() {
        var el = $('.prettyprint');
        el.addClass('.loaded-pretty-print');
        prettyPrint();
    },
    autoAttachFlash: function() {
        $('body').ajaxComplete(function(event, request, settings) {
            var viewer = ADRBasic.viewer;
            var dsid = viewer.dsid;
            if(dsid != null) {
                if(viewer.shouldLoadFlexPaper()) {
                    viewer.loadFlexPaper(dsid);
                }
                else if(viewer.shouldLoadFlowPlayer()) {
                    viewer.loadFlowPlayer(dsid);
                }
                else if(viewer.shouldLoadPrettyPrinter()) {
                    viewer.loadPrettyPrinter();
                }
            }
        });
    }
};