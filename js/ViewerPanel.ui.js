/*
 * File: ViewerPanel.ui.js
 * Date: Mon Apr 04 2011 13:08:10 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ViewerPanelUi = Ext.extend(Ext.Panel, {
    title: 'Viewer',
    height: 250,
    autoHeight: true,
    forceLayout: true,
    html: '<br/>',
    id: 'adr-viewer',
    initComponent: function() {
        this.items = [
            {
                xtype: 'panel',
                title: 'Files',
                frame: true,
                items: [
                    {
                        xtype: 'dataview',
                        itemSelector: 'div',
                        store: 'Datastreams',
                        tpl: [
                            '<tpl for=".">',
                            '  <img src="{img_url}" style="width:50px;height:50px;"></img>',
                            '</tpl>'
                        ]
                    }
                ],
                bbar: {
                    xtype: 'paging',
                    store: 'Datastreams'
                }
            }
        ];
        ViewerPanelUi.superclass.initComponent.call(this);
    }
});
