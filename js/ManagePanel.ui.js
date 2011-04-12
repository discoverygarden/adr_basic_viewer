/*
 * File: ManagePanel.ui.js
 * Date: Tue Apr 12 2011 07:03:16 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ManagePanelUi = Ext.extend(Ext.Panel, {
    title: 'Manage',
    layout: 'border',
    initComponent: function() {
        this.defaults = {
            split: true
        };
        this.items = [
            {
                xtype: 'panel',
                title: 'Detailed Information',
                frame: true,
                width: 200,
                tpl: [
                    '<img src="{img_url}" style="display:block;margin-right:auto;margin-left:auto;"></img>',
                    '<div style="text-align:center;">{label}</div>',
                    '<div style="text-align:center;">{dsid}</div>'
                ],
                region: 'east',
                html: '<div>Select a File</div>',
                id: 'adr-manage-detailed-info'
            },
            {
                xtype: 'form',
                title: 'Manage Files',
                width: 100,
                height: 700,
                layout: 'border',
                region: 'center',
                id: 'adr-manage-files',
                tbar: {
                    xtype: 'toolbar',
                    items: [
                        {
                            xtype: 'button',
                            text: 'Add',
                            width: 50,
                            cls: 'x-btn-text-icon',
                            iconCls: 'adr-add-file-icon',
                            id: 'adr-edit-file-add'
                        },
                        {
                            xtype: 'tbseparator'
                        },
                        {
                            xtype: 'button',
                            text: 'Edit',
                            disabled: true,
                            cls: 'x-btn-text-icon',
                            iconCls: 'adr-edit-file-icon',
                            width: 50,
                            id: 'adr-edit-file-edit'
                        },
                        {
                            xtype: 'tbseparator'
                        },
                        {
                            xtype: 'button',
                            text: 'View',
                            disabled: true,
                            cls: 'x-btn-text-icon',
                            iconCls: 'adr-view-file-icon',
                            width: 50,
                            id: 'adr-edit-file-view'
                        },
                        {
                            xtype: 'tbseparator'
                        },
                        {
                            xtype: 'button',
                            text: 'Download',
                            disabled: true,
                            cls: 'x-btn-text-icon',
                            iconCls: 'adr-download-file-icon',
                            width: 50,
                            id: 'adr-edit-file-download'
                        },
                        {
                            xtype: 'tbseparator'
                        },
                        {
                            xtype: 'button',
                            text: 'Delete',
                            width: 50,
                            disabled: true,
                            cls: 'x-btn-text-icon',
                            iconCls: 'adr-delete-file-icon',
                            id: 'adr-edit-file-delete'
                        }
                    ]
                },
                items: [
                    {
                        xtype: 'listview',
                        store: 'Datastreams',
                        singleSelect: true,
                        columnWidth: 0.8,
                        width: 760,
                        region: 'center',
                        id: 'adr-manage-file-list',
                        columns: [
                            {
                                xtype: 'lvcolumn',
                                header: 'Label',
                                width: 0.25,
                                dataIndex: 'label'
                            },
                            {
                                xtype: 'lvcolumn',
                                header: 'Datastream ID',
                                dataIndex: 'dsid'
                            },
                            {
                                xtype: 'lvcolumn',
                                header: 'Mime Type',
                                dataIndex: 'mime'
                            }
                        ]
                    }
                ],
                fbar: {
                    xtype: 'paging',
                    store: 'Datastreams',
                    pageSize: 33
                }
            },
            {
                xtype: 'form',
                title: 'Object Properties',
                width: 100,
                height: 150,
                frame: true,
                hideBorders: true,
                footer: true,
                region: 'north',
                id: 'adr-manage-object-properties',
                fbar: {
                    xtype: 'toolbar',
                    items: [
                        {
                            xtype: 'button',
                            text: 'Edit Object',
                            id: 'adr-manage-edit-object'
                        },
                        {
                            xtype: 'button',
                            text: 'Edit Permissions',
                            id: 'adr-manage-edit-object-permissions'
                        },
                        {
                            xtype: 'button',
                            text: 'Delete Object',
                            id: 'adr-manage-delete-object'
                        }
                    ]
                },
                items: [
                    {
                        xtype: 'dataview',
                        itemSelector: 'div',
                        store: 'ObjectProperties',
                        tpl: [
                            '<tpl for=".">',
                            ' <table>',
                            '  <tr><th>Label:</th><td>{label}</td></tr>',
                            '  <tr><th>Created: </th><td>{created}</td></tr>',
                            '  <tr><th>Modified: </th><td>{modified}</td></tr>',
                            '  <tr><th>Owner: </th><td>{owner}</td></tr>',
                            '  <tr><th>State: </th><td>{state}</td></tr>',
                            ' </table>',
                            '</tpl>'
                        ]
                    }
                ]
            }
        ];
        ManagePanelUi.superclass.initComponent.call(this);
    }
});
