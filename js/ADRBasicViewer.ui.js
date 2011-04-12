/*
 * File: ADRBasicViewer.ui.js
 * Date: Tue Apr 12 2011 07:03:16 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ADRBasicViewerUi = Ext.extend(Ext.TabPanel, {
    activeTab: 0,
    width: 960,
    height: 920,
    layoutOnTabChange: true,
    border: false,
    forceLayout: true,
    id: 'adr-tabpanel',
    initComponent: function() {
        this.items = [
            {
                xtype: 'panel',
                title: 'Overview',
                layout: 'border',
                defaults: {
                    split: true,
                    
                },
                id: 'adr-overview-tab',
                items: [
                    {
                        xtype: 'panel',
                        title: 'Description',
                        region: 'center',
                        autoLoad: {
                            url: '/adrbasic/ajax/getDescription',
                            
                        },
                        bodyStyle: '{\npadding: 15px\n}',
                        autoScroll: true,
                        frame: true,
                        id: 'adr-overview-description'
                    },
                    {
                        region: 'east',
                        collapseMode: 'standard',
                        collapsible: true,
                        xtype: 'filePanel'
                    }
                ]
            },
            {
                xtype: 'panel',
                title: 'Viewer',
                layout: 'border',
                defaults: {
                    split: true,
                    
                },
                id: 'adr-viewer-tab',
                items: [
                    {
                        xtype: 'panel',
                        title: 'Viewer',
                        region: 'center',
                        autoLoad: {
                            url: "/adrbasic/ajax/getViewer",
                            params: {
                                pid: decodeURIComponent(window.location.pathname.split('/')[
                                    3
                                ])
                            }
                        },
                        frame: true,
                        autoScroll: true,
                        id: 'adr-viewer'
                    },
                    {
                        region: 'east',
                        collapsible: true,
                        collapseMode: 'standard',
                        xtype: 'filePanel'
                    }
                ]
            }
        ];
        ADRBasicViewerUi.superclass.initComponent.call(this);
    }
});
