(function ($) {
    Drupal.behaviors.islandoraGATrackADR = function(context) {
        $('#islandora-solr-simple-search-form').submit(function() {
            if (typeof _gaq != null && typeof _gaq != "undefined") {
                _gaq.push(['_trackEvent', 'Solr Search', 'clicked', $("input:first").val()]);
            }
        });
    }
})(jQuery);