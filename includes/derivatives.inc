<?php

/**
 * @file
 * Functions deriving images for objects.
 */

/**
 * Creates the derivatives for this content model type.
 *
 * @param AbstractObject $object
 *   The object to add derivatives to.
 */
function islandora_data_create_all_derivatives(AbstractObject $object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s. No image file was uploaded.', array('%s' => $object->id)), 'error');
    return "";
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  $file_uri = file_create_filename("{$file_name}OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  // Temporary.
  $file->status = 0;
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);

  // Add thumbnail.
  $path = drupal_get_path('module', 'islandora_data');
  $ds = $object->constructDatastream('TN', "M");
  $ds->label = 'TN';
  $ds->mimetype = 'image/png';
  $ds->setContentFromFile("$path/images/data.png");
  $object->ingestDatastream($ds);

  // Add test datastream.
  $test_file = file_copy($original_file, 'temporary://' . $file_name . 'APPENDED.' . $ext);
  if (islandora_data_test_add_line($test_file)) {
    islandora_data_add_datastream($object, 'FILE_WITH_APPENDED_LINE', $test_file);
  }

/*
  $tn_file = file_copy($original_file, 'temporary://' . $file_name . 'TN.' . $ext);
  if (islandora_basic_image_scale_image($tn_file, 200, 200)) {
    islandora_basic_image_add_datastream($object, 'TN', $tn_file);
  }
*/
/*
  $medium_file = file_copy($original_file, 'temporary://' . $file_name . 'MEDIUM.' . $ext);
  if (islandora_basic_image_scale_image($medium_file, 500, 700)) {
    islandora_basic_image_add_datastream($object, 'MEDIUM_SIZE', $medium_file);
  }
*/
  file_delete($original_file);
}

/**
 * Scales the given image.
 *
 * @param object $file
 *   The image file to scale.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_data_test_add_line($file) {
  $real_path = drupal_realpath($file->uri);
  // /bin/echo '"1","1","1"' >> $real_path
  $command = '/bin/echo "1,1,1" >> ' . $real_path;
  exec($command, $output, $success);
  // If $command was successful, $success will contain return code 0.
  if ($success === 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 */
function islandora_data_add_datastream(AbstractObject $object, $dsid, $file) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile(drupal_realpath($file->uri));
    $object->ingestDatastream($ds);
    file_delete($file);
  }
  catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}
