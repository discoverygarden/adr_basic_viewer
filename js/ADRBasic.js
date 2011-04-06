/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Ext.onReady(function(){
    Ext.get('content-fedora').update('');
    Ext.get('content-fedora').dom.id = "adr-basic-viewer";
    Ext.QuickTips.init();
    var adrbasic = new ADRBasicViewer({
        renderTo: 'adr-basic-viewer',
        listeners: {
            afterrender: function() {
                $('#center form').css('margin-bottom', 0);
            }
        }
    });
    adrbasic.show();
});