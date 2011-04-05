/*
 * File: AddFileWindow.ui.js
 * Date: Tue Apr 05 2011 20:06:20 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

AddFileWindowUi = Ext.extend(Ext.Window, {
    title: 'Add File',
    width: 637,
    height: 259,
    layout: 'anchor',
    defaultType: 'form',
    header: true,
    initComponent: function() {
        this.items = [
            {
                xtype: 'form',
                title: 'Add File',
                frame: true,
                headerAsText: false,
                animCollapse: false,
                method: 'POST',
                anchor: '100% 100%',
                fileUpload: true,
                items: [
                    {
                        xtype: 'textfield',
                        fieldLabel: 'DSID',
                        anchor: '100%',
                        name: 'dsid',
                        allowBlank: false
                    },
                    {
                        xtype: 'textfield',
                        fieldLabel: 'Label',
                        anchor: '100%',
                        name: 'label',
                        allowBlank: false
                    },
                    {
                        xtype: 'combo',
                        fieldLabel: 'Control Group',
                        anchor: '100%',
                        name: 'control_group',
                        store: 'ControlGroups',
                        displayField: 'type',
                        mode: 'local',
                        forceSelection: true,
                        editable: false,
                        allowBlank: false,
                        triggerAction: 'all',
                        valueField: 'value',
                        submitValue: true,
                        value: 'Please Select...'
                    },
                    {
                        xtype: 'combo',
                        name: 'mime_type',
                        store: 'MIMETypes',
                        title: 'Mime Type',
                        fieldLabel: 'MIME Type',
                        forceSelection: true,
                        triggerAction: 'all',
                        displayField: 'mime',
                        editable: false,
                        allowBlank: false
                    },
                    {
                        fieldLabel: 'Upload File',
                        xtype: 'fileuploadfield'
                    }
                ],
                fbar: {
                    xtype: 'toolbar',
                    autoHeight: true,
                    forceLayout: true,
                    height: 28,
                    id: 'add-file-form-toolbar',
                    items: [
                        {
                            xtype: 'button',
                            text: 'Add'
                        }
                    ]
                }
            }
        ];
        AddFileWindowUi.superclass.initComponent.call(this);
    }
});
