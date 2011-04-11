/*
 * File: EditObjectWindow.ui.js
 * Date: Mon Apr 11 2011 07:07:39 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

EditObjectWindowUi = Ext.extend(Ext.Window, {
    title: 'Edit Object',
    width: 453,
    height: 181,
    layout: 'anchor',
    initComponent: function() {
        this.items = [
            {
                xtype: 'form',
                title: 'Edit Object',
                headerAsText: false,
                anchor: '100% 100%',
                header: true,
                frame: true,
                items: [
                    {
                        xtype: 'textfield',
                        fieldLabel: 'Label',
                        anchor: '100%',
                        name: 'label'
                    },
                    {
                        xtype: 'textfield',
                        fieldLabel: 'Owner',
                        anchor: '100%',
                        name: 'owner'
                    },
                    {
                        xtype: 'combo',
                        fieldLabel: 'State',
                        name: 'state',
                        store: 'State',
                        displayField: 'type',
                        valueField: 'value',
                        value: 'Please Select...',
                        submitValue: true,
                        mode: 'local',
                        allowBlank: false,
                        editable: false,
                        triggerAction: 'all',
                        forceSelection: true
                    }
                ],
                fbar: {
                    xtype: 'toolbar',
                    items: [
                        {
                            xtype: 'button',
                            text: 'Save'
                        }
                    ]
                }
            }
        ];
        EditObjectWindowUi.superclass.initComponent.call(this);
    }
});
