/*
 * File: ReplaceFileWindow.ui.js
 * Date: Thu Nov 17 2011 14:44:11 GMT-0400 (AST)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ReplaceFileWindowUi = Ext.extend(Ext.Window, {
    title: 'Replace File',
    width: 400,
    height: 250,
    initComponent: function() {
        this.items = [
            {
                xtype: 'form',
                title: 'Replace File',
                headerAsText: false,
                fileUpload: true,
                autoHeight: true,
                maskDisabled: false,
                frame: true,
                items: [
                    {
                        fieldLabel: 'Upload File',
                        xtype: 'fileuploadfield'
                    }
                ]
            }
        ];
        ReplaceFileWindowUi.superclass.initComponent.call(this);
    }
});
