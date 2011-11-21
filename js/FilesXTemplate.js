/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


var filesXTemplate = new Ext.XTemplate(
    '<tpl for=".">',
    '   <div class="adr-file">',
    '       <div class="adr-file-dsid">{[fm.ellipsis(values.dsid, 30, true)]}</div>',
    '       <tpl if="this.showLabel(label)">',
    '           <div class="adr-file-label">{[fm.ellipsis(values.label, 30, true)]}</div>',
    '       </tpl>',
    '       <img class="adr-file-img adr-file-show-view" src="{img_url}" onLoad="resizeImage(this, 96);"></img>',
    '       <div class="adr-file-controls">',
    '           <span class="adr-file-view">',
    '               <tpl if="this.showView(mime)">',
    '                   <input type="button" onClick="ADRBasic.viewer.show(\'{label}\', \'{viewable_dsid}\');" value="View"/>',
    '               </tpl>',
    '               <tpl if="this.showPlay(mime)">',
    '                   <input type="button" onClick="ADRBasic.viewer.show(\'{label}\', \'{viewable_dsid}\');" value="Play"/>',
    '               </tpl>',
    '           </span>',
    '           <tpl if="download_url">',
    '               <span class="adr-file-download">',
    '                   <form method="GET" action="{download_url}" style="display:inline;">',
    '                       <input type="submit" value="Download"></input>',
    '                   </form>',
    '               </span>',
    '           </tpl>',
    '       </div>',
    '   </div>',
    '</tpl>',
    {
        // XTemplate configuration:
        compiled: true,
        disableFormats: true,
        // member functions:
        showLabel: function(label) {
            return jQuery.trim(label) != '';
        },
        showView: function(mime){
            return mime.indexOf("audio") == -1 && mime.indexOf("video") == -1;
        },
        showPlay: function(mime){
            return mime.indexOf("audio") != -1 || mime.indexOf("video") != -1;
        }
    }
    );