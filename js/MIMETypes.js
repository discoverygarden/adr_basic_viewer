/*
 * File: MIMETypes.js
 * Date: Fri Mar 18 2011 13:12:53 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.0.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

MIMETypes = Ext.extend(Ext.data.JsonStore, {
    constructor: function(cfg) {
        cfg = cfg || {};
        MIMETypes.superclass.constructor.call(this, Ext.apply({
            storeId: 'MIMETypes',
            url: '/adrbasic/ajax/getAllowedMIMETypes',
            autoLoad: true,
            baseParams: {
                pid: (window.location.pathname.split('/')[
                    3
                ])
            },
            fields: [
                {
                    name: 'mime'
                }
            ]
        }, cfg));
    }
});
new MIMETypes();