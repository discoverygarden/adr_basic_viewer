/*
 * File: OverviewPanel.ui.js
 * Date: Wed Mar 16 2011 16:32:48 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version xds-1.0.3.2.
 * http://www.extjs.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

OverviewPanelUi = Ext.extend(Ext.Panel, {
    title: 'Overview',
    layout: 'column',
    border: false,
    hideBorders: true,
    initComponent: function() {
        this.items = [
            {
                flex: 1,
                layout: 'fit',
                columnWidth: 0.7,
                xtype: 'descriptionpanel'
            },
            {
                columnWidth: 0.3,
                layout: 'fit',
                xtype: 'filespanel'
            }
        ];
        OverviewPanelUi.superclass.initComponent.call(this);
    }
});
