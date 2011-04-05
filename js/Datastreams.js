/*
 * File: Datastreams.js
 * Date: Tue Apr 05 2011 20:06:21 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

Datastreams = Ext.extend(Ext.data.JsonStore, {
    constructor: function(cfg) {
        cfg = cfg || {};
        Datastreams.superclass.constructor.call(this, Ext.apply({
            storeId: 'Datastreams',
            url: '/adrbasic/ajax/getDatastreams',
            root: 'data',
            baseParams: {
                pid: decodeURIComponent(window.location.pathname.split('/')[
                    3
                ])
            },
            fields: [
                {
                    name: 'dsid'
                },
                {
                    name: 'label'
                },
                {
                    name: 'img_url'
                },
                {
                    name: 'mime'
                },
                {
                    name: 'download_url'
                }
            ]
        }, cfg));
    }
});
new Datastreams();