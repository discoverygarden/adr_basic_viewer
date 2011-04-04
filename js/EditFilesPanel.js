/*
 * File: EditFilesPanel.js
 * Date: Mon Mar 07 2011 02:10:59 GMT-0600 (CST)
 * 
 * This file was generated by Ext Designer version xds-1.0.3.2.
 * http://www.extjs.com/products/designer/
 *
 * This file will be generated the first time you export.
 *
 * You should implement event handling and custom methods in this
 * class.
 */

function gotoEditModsPage() {
    var pid = window.location.pathname.split('/')[3];
    var location = window.location;
    var page = location.protocol + '//' + location.host + '/formbuilder/edit/' + pid + '/MODS/colorado';
    window.location = page;
}

EditFilesPanel = Ext.extend(EditFilesPanelUi, {
    initComponent: function() {
        EditFilesPanel.superclass.initComponent.call(this);
        var store = Ext.StoreMgr.lookup('Datastreams');
        store.reload(store.lastOptions);
        /*
        var viewer = this.get('edit-files-panel-data-viewer');
        var add = this.buttons[0];
        var edit = this.buttons[1];
        var view = this.buttons[2];
        var remove = this.buttons[3];

        add.addListener('click', function(button, event) {
            var window = new AddFileWindow();
            window.show(this);
        });
        remove.addListener('click', function(button, event) {
            var records = viewer.getSelectedRecords();
            var record = records[0];
            if(record) {
                var store = viewer.getStore();
                var pid = store.baseParams.pid;
                var dsid = record.get('dsid');
                Ext.Msg.confirm('Delete', 'Are you sure you want to delete this file?', function(btn, text){
                    if (btn == 'yes') {
                        Ext.Ajax.request({
                            url: '/adrbasic/ajax/removeDatastream',
                            success: function() {
                                var store = Ext.StoreMgr.lookup('Description');
                                store.reload(store.lastOptions);
                                store = Ext.StoreMgr.lookup('Datastreams');
                                store.reload(store.lastOptions);
                                store = Ext.StoreMgr.lookup('OverviewDatastreams');
                                store.reload(store.lastOptions);
                            },
                            failure: function() {
                                Ext.Msg.alert('Failure', 'Could not delete file.');
                            },
                            params: {
                                pid: pid,
                                dsid: dsid
                            }
                        });

                    }
                });
            }

        });
        view.addListener('click', function(button, event) {
            var records = viewer.getSelectedRecords();
            var record = records[0];
            if(record) {
                var store = viewer.getStore();
                var pid = store.baseParams.pid;
                var dsid = record.get('dsid');
                var viewerPanel = Ext.getCmp('adr-viewer');
                if(viewerPanel.rendered) {
                    var loadOptions = viewerPanel.autoLoad;
                    loadOptions.params.dsid = dsid;
                    viewerPanel.load(loadOptions);
                    ADRBasic.viewerDSID = dsid;
                    viewerPanel.show();
                }
                else {
                    ADRBasic.viewerDSID = dsid;
                    viewerPanel.autoLoad.params.dsid = dsid;
                    viewerPanel.show();
                }
            }
        });
        viewer.addListener('click', function(dataviewer, index, node, event) {
            var record = dataviewer.getStore().getAt(index);
            if(record) {
                var dsid = record.get('dsid');
                if(dsid == 'MODS') {
                    edit.enable();
                    edit.addListener('click', gotoEditModsPage);
                }
                else {
                    edit.disable();
                    edit.removeListener('click', gotoEditModsPage);
                }
                remove.enable();
                view.enable();
            }
            else {
                edit.disable();
                remove.disable();
                view.disable();
            }
        });*/
    }
});
Ext.reg('editfilespanel', EditFilesPanel);