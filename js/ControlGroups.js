/*
 * File: ControlGroups.js
 * Date: Tue Apr 05 2011 20:06:21 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

ControlGroups = Ext.extend(Ext.data.ArrayStore, {
    constructor: function(cfg) {
        cfg = cfg || {};
        ControlGroups.superclass.constructor.call(this, Ext.apply({
            storeId: 'ControlGroups',
            data: [
                [
                    "Inline XML (X)",
                    "X"
                ],
                [
                    "Managed Resource (M)",
                    "M"
                ],
                [
                    "Redirect (R)",
                    "R"
                ],
                [
                    "External Reference (E)",
                    "E"
                ]
            ],
            fields: [
                {
                    name: 'type',
                    type: 'string'
                },
                {
                    name: 'value'
                }
            ]
        }, cfg));
    }
});
new ControlGroups();