/*
 * File: EditObjectPanel.ui.js
 * Date: Mon Apr 04 2011 12:05:00 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

EditObjectPanelUi = Ext.extend(Ext.form.FormPanel, {
    title: 'Object Properties',
    frame: true,
    hideBorders: true,
    footer: true,
    initComponent: function() {
        this.fbar = {
            xtype: 'toolbar',
            items: [
                {
                    xtype: 'button',
                    text: 'Edit Object'
                },
                {
                    xtype: 'button',
                    text: 'Edit Permissions'
                },
                {
                    xtype: 'button',
                    text: 'Delete Object'
                }
            ]
        };
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: 'div',
                store: 'ObjectProperties',
                tpl: [
                    '<tpl for=".">',
                    ' <table>',
                    '  <tr><td>Label</td><td>{label}</td></tr>',
                    '  <tr><td>Created</td><td>{created}</td></tr>',
                    '  <tr><td>Modified</td><td>{modified}</td></tr>',
                    '  <tr><td>Owner</td><td>{owner}</td></tr>',
                    '  <tr><td>State</td><td>{state}</td></tr>',
                    ' </table>',
                    '</tpl>'
                ]
            }
        ];
        EditObjectPanelUi.superclass.initComponent.call(this);
    }
});
