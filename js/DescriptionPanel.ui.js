/*
 * File: DescriptionPanel.ui.js
 * Date: Mon Apr 04 2011 12:05:00 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

DescriptionPanelUi = Ext.extend(Ext.Panel, {
    title: 'Description',
    layout: 'table',
    autoHeight: true,
    forceLayout: true,
    border: false,
    initComponent: function() {
        this.autoLoad = {
            url: '/adrbasic/ajax/getDescription',
            params: {
                pid: (window.location.pathname.split('/')[
                    3
                ])
            }
        };
        DescriptionPanelUi.superclass.initComponent.call(this);
    }
});
