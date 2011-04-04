/*
 * File: EditFilesPanel.ui.js
 * Date: Mon Apr 04 2011 13:08:10 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

EditFilesPanelUi = Ext.extend(Ext.form.FormPanel, {
    title: 'Manage Files',
    frame: true,
    initComponent: function() {
        this.fbar = {
            xtype: 'toolbar',
            id: 'edit-files-panel-toolbar',
            items: [
                {
                    xtype: 'button',
                    text: 'Add',
                    width: 75
                },
                {
                    xtype: 'button',
                    text: 'Edit',
                    disabled: true
                },
                {
                    xtype: 'button',
                    text: 'View',
                    disabled: true
                },
                {
                    xtype: 'button',
                    text: 'Delete',
                    width: 75,
                    disabled: true
                }
            ]
        };
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: 'div .adr-edit-file-img-block',
                store: 'Datastreams',
                tpl: [
                    '<tpl for=".">',
                    '<div class="adr-edit-file-img-block">',
                    '<img src=\'{img_url}\' title=\'label\' class="adr-edit-file-img">',
                    '<div class="adr-edit-file-label">{dsid}</div></div>',
                    '</tpl>'
                ],
                overClass: 'x-view-over',
                singleSelect: true,
                trackOver: true,
                id: 'edit-files-panel-data-viewer'
            }
        ];
        EditFilesPanelUi.superclass.initComponent.call(this);
    }
});
