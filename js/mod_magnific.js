(function($){
    $(document).ready(function(){
        $('.popup-video').each(function(inde,e) {
            var id_video = $(this).attr('id');
            $('#'+id_video).magnificPopup({
                disableOn: 700,
                delegate: 'a',
                type: 'iframe',
                mainClass: 'mfp-fade',
                removalDelay: 160,
                preloader: false,
                fixedContentPos: false,
                mainClass: "animated " + Drupal.settings.mod_magnific.animate_video,
                gallery: {
                    enabled: true,
                    navigateByImgClick: true,
                    preload: [0, 1] // Will preload 0 - before current, and 1 after the current image
                },
            });
        });
        $('.popup-image').each(function(inde,e){
            var id_img = $(this).attr('id');
            $('#'+id_img).magnificPopup({
                delegate: 'a',
                type: 'image',
                gallery: {
                    enabled: true
                },
                removalDelay: 300,
                mainClass:"animated "+ Drupal.settings.mod_magnific.animate_image,
            });
        });
    })
}(jQuery));