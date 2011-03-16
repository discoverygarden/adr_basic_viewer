/*
 * File: DescriptionPanel.ui.js
 * Date: Wed Mar 16 2011 00:53:16 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version xds-1.0.3.2.
 * http://www.extjs.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

DescriptionPanelUi = Ext.extend(Ext.Panel, {
    title: 'Description',
    layout: 'fit',
    initComponent: function() {
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: 'div',
                store: 'Description',
                tpl: [
                    '<tpl for=".">',
                    '<div class="adr-title-label">Title:</div><div class="adr-title-value">{title}</div><br clear="all"/>',
                    '<div class="adr-author-label">Author:</div><div class="adr-author-value">{author}</div><br clear="all"/>',
                    '<div class="adr-description-label">Description:</div><div class="adr-description-value">{description}</div><br clear="all"/>',
                    '<div class="adr-overview-footer">This object was added {date_created}. This object was last updated {date_modified} by {user}.</div>',
                    '</tpl>'
                ]
            }
        ];
        DescriptionPanelUi.superclass.initComponent.call(this);
    }
});
