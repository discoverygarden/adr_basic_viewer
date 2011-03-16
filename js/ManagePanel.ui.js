/*
 * File: ManagePanel.ui.js
 * Date: Wed Mar 16 2011 16:32:48 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version xds-1.0.3.2.
 * http://www.extjs.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ManagePanelUi = Ext.extend(Ext.Panel, {
    title: 'Manage',
    layout: 'vbox',
    defaults: '',
    defaultType: 'auto',
    frame: true,
    initComponent: function() {
        this.layoutConfig = {
            align: 'stretch'
        };
        this.items = [
            {
                flex: 1,
                margins: '0 0 10 0',
                xtype: 'editobjectpanel'
            },
            {
                flex: 2,
                margins: '0 0 10 0',
                xtype: 'editdescriptionpanel'
            },
            {
                flex: 4,
                margins: '0 0 5 0',
                xtype: 'editfilespanel'
            }
        ];
        ManagePanelUi.superclass.initComponent.call(this);
    }
});
