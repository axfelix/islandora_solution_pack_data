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

  // Add CSV datastream(s). There will be one per worksheet in the Excel and OpenOffice Calc files.
  $file_name = str_replace(":", "-", $object->id) . ".OBJ.{$ext}";
  $file_uri = file_create_filename($file_name, 'temporary://');
  // Populate the OBJ datastream with the uploaded file.
  $object['OBJ']->getContent($file_uri);
  islandora_data_get_csv_version($file_uri, $ext);
  // The current process ID was passed to csvnormalize.py as a parameter so it
  // knows what output file to write to. Here, we grab the process ID so we can
  // read that file.
  $current_process_id = getmypid();
  $csvlist_filepath = '/tmp/csvlist.' . $current_process_id;
  $fh = fopen($csvlist_filepath, 'r');
  while ($line = fgets($fh)) {
    $file_basename = basename($line);
    // Add each datastream in the 'csvlist' file. Each one will be named after the 
    // worksheet from which it is derived, e.g. 'CSV_DATA_foo', 'CSV_DATA_bar', etc.
    islandora_data_add_datastream($object, 'CSV_DATA_' . $file_basename, $line);
  }
  fclose($fh);
  // During development, don't delete these files. In production, delete them.
  // file_delete($csvlist_filepath);
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
      
      // Get the current process ID so we can ensure a unique filename for the output
      // of csvnormalize.py.
      $current_process_id = getmypid();
      $command = 'python csvnormalize.py ' . $input_file_path . ' ' . $current_process_id;
      exec($command, $output, $ret);
    case 'csv':
      file_put_contents('csvlist', $input_file_path);
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
