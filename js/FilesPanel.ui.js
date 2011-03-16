/*
 * File: FilesPanel.ui.js
 * Date: Wed Mar 16 2011 16:39:12 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version xds-1.0.3.2.
 * http://www.extjs.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

FilesPanelUi = Ext.extend(Ext.Panel, {
    title: 'Files',
    initComponent: function() {
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: 'img',
                store: 'Datastreams',
                tpl: [
                    '<tpl for=".">',
                    '<table width="100%" border="1">',
                    '  <tr>',
                    '    <td style="vertical-align: middle;"><img src=\'{img_url}\' title="{label}" class="adr-file-img"/></td>',
                    '    <td style="vertical-align: middle;">',
                    '       <div class="adr-file-label">{dsid}</div>',
                    '       <div>',
                    '          <form method="GET" action="{download_url}">',
                    '             <input type="submit" value="Download"></input>',
                    '          </form>',
                    '       </div>',
                    '    </td>',
                    '  </tr>',
                    '</tpl>'
                ],
                singleSelect: true,
                trackOver: true,
                overClass: 'x-view-over',
                id: 'adr-files'
            }
        ];
        FilesPanelUi.superclass.initComponent.call(this);
    }
});
