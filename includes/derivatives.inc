<?php

/**
 * @file
 * Functions for creating derivatives of the Excel, CSV, etc. files.
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
    drupal_set_message(t('Could not normalize %s. No valid file was uploaded.', array('%s' => $object->id)), 'error');
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
  $file->status = 0; // Temporary file.
  $object['OBJ']->getContent($file_uri);
  $original_file = file_save($file);

  // Add thumbnail.
  $path = drupal_get_path('module', 'islandora_data');
  $ds = $object->constructDatastream('TN', "M");
  $ds->label = 'TN';
  $ds->mimetype = 'image/png';
  $ds->setContentFromFile("$path/images/data.png");
  $object->ingestDatastream($ds);

  // Add CSV datastream.
  $file_name = str_replace(":", "-", $object->id) . ".OBJ.{$ext}";
  $file_uri = file_create_filename($file_name, 'temporary://');
  // Populate the OBJ datastream with the uploaded file.
  $object['OBJ']->getContent($file_uri);
  if (($output_file_uri = islandora_data_get_csv_version($file_uri, $ext)) !== FALSE) {
      $fh = fopen('filename.txt','r');
      while ($line = fgets($fh)) {
      islandora_data_add_datastream($line, 'CSV_DATA', $output_file_uri);
    }
    fclose($fh)
  }
  file_unmanaged_delete($file_uri);

  file_delete($original_file);
}

/**
 * Extract the CSV content from the uploaded file.
 *
 * @param string $file_uri
 *   The Drupal URI of a copy of the OBJ file to extract CSV content from.
 * @param string $ext
 *   The extension of the file.
 *
 * @return string $output_file_path
 *   The absolute path to the output file.
 */
function islandora_data_get_csv_version($file_uri, $ext) {
  $output = array();
  $input_file_path = drupal_realpath($file_uri);
  $output_file_path = $input_file_path . '.csv';
  
  // During development, apply the linux 'file' command to .xls, .xlsx,
  // and .ods files and redirect the output to $output_file_path. Return the
  // path to this file so the file can be used as the content of the CSV_DATA
  // datastream. For .csv files, return the original file path, so the .csv file
  // will be used as the content of the CSV_DATA datastream.
  switch ($ext) {
    case 'xls':
    case 'xlsx':
    case 'ods':
      // Extract the CSV content from files with these extensions.
      // The output of your extraction command needs to write the results to
      // $real_path, i.e., the input and output paths are the same. If this is
      // a problem, you will need to make a temporary copy of the file for input.
      
      // During development, we just dump the output of the 'file' command
      // to the 'csv' output file.
      $command = 'python csvnormalize.py ' . $input_file_path;
      exec($command, $output, $ret);
      // If $command was successful, $success will contain return code 0.
      if ($ret == '0') {
        return $output_file_path;
      }
    case 'csv':
      // During development, don't process the input file, just return its path.
      return $input_file_path;
      
    default:
      // I.e., none of the cases apply.
      return FALSE;
  }
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file_uri
 *   The URI (full Drupal path) of the file to be added as the datastream content.
 */
function islandora_data_add_datastream(AbstractObject $object, $dsid, $file_uri) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    $ds->mimeType = $object['OBJ']->mimeType;
    $ds->setContentFromFile($file_uri);
    $object->ingestDatastream($ds);
  }
  catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}
