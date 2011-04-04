/*
 * File: Files.ui.js
 * Date: Mon Apr 04 2011 18:37:41 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

FilesUi = Ext.extend(Ext.Panel, {
    title: 'Files',
    width: 230,
    initComponent: function() {
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: 'img.adr-file-img',
                store: 'OverviewDatastreams',
                tpl: [
                    '<tpl for=".">',
                    '<div class="adr-file">',
                    '<div class="adr-file-label">{[fm.ellipsis(values.label, 30, true)]}</div>',
                    '<img class="adr-file-img" src="{img_url}"></img>',
                    '<div class="adr-file-dsid">{[fm.ellipsis(values.dsid, 30, true)]}</div>',
                    '<div class="adr-file-controls">',
                    '</div>',
                    '</div>',
                    '</tpl>'
                ],
                emptyText: 'Not Specified...',
                loadingText: 'loading...',
                singleSelect: true,
                trackOver: true,
                overClass: 'adr-file-img-over'
            }
        ];
        this.fbar = {
            xtype: 'paging',
            store: 'OverviewDatastreams',
            pageSize: 3
        };
        FilesUi.superclass.initComponent.call(this);
    }
});
