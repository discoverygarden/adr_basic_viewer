function resizeImage(e, maxSize) {

    var height = $(e).height();
    var width = $(e).width();
    if (height > width)
    {
        if ( height > maxSize)
        {
            $(e).height(maxSize);
            $(e).width( maxSize*width/height );
        }

    }
    else{
        if (width > maxSize)
        {
            $(e).width(maxSize);
            $(e).height(maxSize*height/width);
            $(e).css('margin-top',  (maxSize-$(e).height())/2+'px');
        }

    }
}