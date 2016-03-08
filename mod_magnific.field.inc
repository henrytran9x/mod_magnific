<?php

/**
 * Implements hook_field_formatter_info().
 */
function mod_magnific_field_formatter_info()
{
    return array(
        'mod_magnific' => array(
            'label' => t('Magnific'),
            'field types' => array('file'),
            'settings' => array(
                'image_style' => '',
                'group_video' => array(
                    'animate_video' => '',
                    'image_style_video' => '',
                ),
                'group_image' => array(
                    'animate_image' => '',
                    'image_style_image' => '',
                ),
            ),
        )
    );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function mod_magnific_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state)
{
    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];
    $image_styles = image_style_options(FALSE, PASS_THROUGH);
    $list_animate = mod_magnific_list_style();

    

    $element['group_video'] = array(
        '#type' => 'fieldset',
        '#title' => t('Popup Video animate'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
    );

    $element['group_video']['animate_video'] = array(
        '#type' => 'select',
        '#default_value' => $settings['group_video']['animate_video'],
        '#empty_option' => t('Nothing'),
        '#options' => $list_animate
    );
     $element['group_video']['image_style_video'] = array(
        '#title' => t('Image style video'),
        '#type' => 'select',
        '#default_value' => $settings['group_video']['image_style_video'],
        '#empty_option' => t('None (original image)'),
        '#options' => $image_styles,
    );
    $element['group_image'] = array(
        '#type' => 'fieldset',
        '#title' => t('Popup Image animate'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
    );
    $element['group_image']['animate_image'] = array(
        '#type' => 'select',
        '#default_value' => $settings['group_image']['animate_image'],
        '#empty_option' => t('Nothing'),
        '#options' => $list_animate
    );
    $element['group_image']['image_style_image'] = array(
        '#title' => t('Image style image'),
        '#type' => 'select',
        '#default_value' => $settings['group_image']['image_style_image'],
        '#empty_option' => t('None (original image)'),
        '#options' => $image_styles,
    );

    return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function mod_magnific_field_formatter_settings_summary($field, $instance, $view_mode)
{

    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];
    return t('Setting magnific') ;
}

/**
 * Implements hook_field_formatter_view().
 */
function mod_magnific_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display)
{
    $element = array();
    $settings = $display['settings'];
    krsort($items);
    foreach ($items as $delta => $item) {

        if($item['type'] == 'video')
        {
            $element = array(
                '#theme' => 'mod_magnific_video',
                '#items' => $items,
                '#animate_style' => $settings['group_video']['animate_video'],
                '#image_style_video' => $settings['group_video']['image_style_video'],
            );
        }
        if($item['type'] == 'image') {
            $element = array(
                '#theme' => 'mod_magnific_image',
                '#items' => $items,
                '#animate_style' => $settings['group_image']['animate_image'],
                '#image_style_image' => $settings['group_image']['image_style_image'],
            );
        }
    }
    drupal_add_js(array('mod_magnific' => array('animate_video' => $settings['group_video']['animate_video'],'animate_image'=>$settings['group_image']['animate_image'])), array('type' => 'setting'));
    drupal_add_css(drupal_get_path('module','mod_magnific').'/css/magnific-popup.css');
    drupal_add_css(drupal_get_path('module','mod_magnific').'/css/mod_magnific.css');
    drupal_add_js(drupal_get_path('module','mod_magnific').'/js/magnific-popup.min.js');
    drupal_add_js(drupal_get_path('module','mod_magnific').'/js/mod_magnific.js');
    return $element;
}

function theme_mod_magnific_video(&$variables)
{
    $items = $variables['items'];
    $output = '';
    module_load_include('inc', 'media_youtube', '/includes/media_youtube.formatters.inc');
    $display['settings'] = isset($variables['image_style_video']) ? array('image_style' => $variables['image_style_video'])  : array('image_style' => 'none');
    $output.= '<div class="gallery-item">';
        // Load the file
        $file = file_load($items[0]['fid']);
        // Get the render array for the thumbnail image
        $youtube = media_youtube_file_formatter_image_view($file, $display, LANGUAGE_NONE);
    $output .= render($youtube);
    $id= drupal_html_id('popup-video');
    $output .= '<div class="hover-mask-container">
                    <div class="hover-mask"></div>
                    <div id="'.$id.'" class="hover-zoom popup-video">';
    foreach($items as $key => $item) {
        $url_video = file_create_url($item['uri']);
        //Set element first
        $class = ($key == 0 ? 'fa fa-play-circle' : 'hidden');
        $output.='<a href="' . $url_video . '" class="'.$class.'"></a>';
    }
    $output.='  </div>
		      </div>
            </div>';
    return $output;

}

function theme_mod_magnific_image(&$variables)
{
    $items  = $variables['items'];
    $output = '';
    $image = array(
        'path' => $items[0]['uri'],
    );

    if (array_key_exists('alt', $items[0])) {
        $image['alt'] = $items[0]['alt'];
    }

    if (isset($items[0]['attributes'])) {
        $image['attributes'] = $items[0]['attributes'];
    }

    if (isset($items[0]['width']) && isset($items[0]['height'])) {
        $image['width'] = $items[0]['width'];
        $image['height'] = $items[0]['height'];
    }

    // Do not output an empty 'title' attribute.
    if (isset($items[0]['title']) && drupal_strlen($items[0]['title']) > 0) {
        $image['title'] = $items[0]['title'];
    }

    if ($variables['image_style_image']) {
        $image['style_name'] = $variables['image_style_image'];
        $image_output = theme('image_style', $image);
    }
    else {
        $image_output = theme('image', $image);
    }
    // The link path and link options are both optional, but for the options to be
    // processed, the link path must at least be an empty string.
    $id = drupal_html_id('popup-image');
    $output.= '<div class="gallery-item">';
    $output .= render($image_output);
    $output .= '<div class="hover-mask-container">
                    <div class="hover-mask"></div>
                    <div id="'.$id.'" class="hover-zoom popup-image">';
    foreach($items as $key => $item) {
        $url_img = file_create_url($item['uri']);
        $class = ($key == 0 ? 'fa fa-image' : 'hidden');
        $output.='<a href="' . $url_img . '" class="'.$class.'"></a>';
    }
    $output.='  </div>
		      </div>
            </div>';
    return $output;
}

