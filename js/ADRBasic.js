/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

function getViewerDSID() {
    if(!ADRBasic.viewerDSID) {
        var store = Ext.StoreMgr.lookup('OverviewDatastreams');
        var record = store.getAt(0);
        ADRBasic.viewerDSID = record.get('dsid');
    }
    return ADRBasic.viewerDSID;

}

// Ugly global stuff cause of deadline...
$(document).ready(function() { 
    $('body').ajaxComplete(function(event, request, settings) {
        var el = $('#playerFLV');
        if(el.length != 0 && !$(el).hasClass('.loaded-flv-player')) {
            el.addClass('.loaded-flv-player');
            loadFLVPlayer(getViewerDSID());
        }
        el = $('#playerFlexPaper');
        if(el.length != 0 && !$(el).hasClass('.loaded-flex-player')) {
            el.addClass('.loaded-flex-player');
            loadFlexPlayer(getViewerDSID());
        }
        el = $('#playerFlow');
        if(el.length != 0 && !$(el).hasClass('.loaded-flow-player')) {
            el.addClass('.loaded-flow-player');
            loadFlowPlayer(getViewerDSID());
        }
        var el = $('.prettyprint');
        if(el.length != 0 && !$(el).hasClass('.loaded-pretty-print')) {
            el.addClass('.loaded-pretty-print');
            prettyPrint();
        }
    });
});

Ext.onReady(function(){
    $('#content-fedora').empty();
    Ext.QuickTips.init();
    var adrbasic = new ADRBasicViewer({
        renderTo: 'content-fedora',
        listeners: {
            afterrender: function() {
                $('#center form').css('margin-bottom', 0);
            }
        }
    });
    adrbasic.show();
});