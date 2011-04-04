/*
 * File: FilesPanel.ui.js
 * Date: Mon Apr 04 2011 13:08:10 GMT-0300 (ADT)
 * 
 * This file was generated by Ext Designer version 1.1.2.
 * http://www.sencha.com/products/designer/
 *
 * This file will be auto-generated each and everytime you export.
 *
 * Do NOT hand edit this file.
 */

FilesPanelUi = Ext.extend(Ext.Panel, {
    title: 'Files',
    autoScroll: true,
    frame: true,
    footer: true,
    initComponent: function() {
        this.items = [
            {
                xtype: 'dataview',
                itemSelector: '.adr-files-show-viewer',
                store: 'OverviewDatastreams',
                tpl: [
                    '<tpl for=".">',
                    '<table width="100%" border="0" class="adr-file-img-block">',
                    '   <tr>',
                    '    <td style="vertical-align: middle;" class="adr-overview-img adr-files-show-viewer" colspan="2"><img src=\'{img_url}\' title="{label}" class="adr-file-img"/></td>',
                    '   </tr>',
                    '   <tr>',
                    '     <td colspan="2">',
                    '       <div class="adr-file-label">{label}</div>',
                    '     </td>',
                    '   </tr>',
                    '   <tr>',
                    '     <td colspan="2">',
                    '       <div class="adr-file-label">{dsid}</div>',
                    '     </td>',
                    '   </tr>',
                    '    <tr>',
                    '       <td colspan="1"><input type="submit" value="View" class="adr-file-view-button adr-files-show-viewer" style="display:block;"></input></td>',
                    '       <td colspan="1">',
                    '           <form method="GET" action="{download_url}">',
                    '                <input type="submit" value="Download" class="adr-file-download-button" style="display:block;"></input>',
                    '           </form>',
                    '       </td>',
                    '     </tr>',
                    ' </table>',
                    '</tpl>'
                ],
                singleSelect: true,
                autoShow: true,
                selectedClass: '',
                id: 'adr-files'
            }
        ];
        this.fbar = {
            xtype: 'paging',
            store: 'OverviewDatastreams',
            pageSize: 5
        };
        FilesPanelUi.superclass.initComponent.call(this);
    }
});
