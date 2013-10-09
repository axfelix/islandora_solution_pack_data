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

  // Create a file object.
  $file_uri = file_create_filename("{$file_name}.OBJ.{$ext}", 'temporary://');
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['OBJ']->mimeType;
  $file->status = 0; // Temporary file.
  // Populate the OBJ datastream.
  $object['OBJ']->getContent($file_uri);
  // Save the file.
  $original_file = file_save($file);

  // Add thumbnail datastream.
  $path = drupal_get_path('module', 'islandora_data');
  $ds = $object->constructDatastream('TN', "M");
  $ds->label = 'TN';
  $ds->mimetype = 'image/png';
  $ds->setContentFromFile("$path/images/data.png");
  $object->ingestDatastream($ds);

  // Add CSV datastream(s). There will be one per worksheet in the Excel and OpenOffice Calc files.
  // The current process ID was passed to csvnormalize.py as a parameter so it
  // knows what output file to write to. Here, we grab the process ID so we can
  // read that file.
  $current_process_id = getmypid();

  $derivs_source_filepath = file_unmanaged_copy($original_file->uri, 'temporary://' . $file_name . '.derivs.' . $ext, FILE_EXISTS_REPLACE);
  $csvlist_filepath = '/tmp/csvlist.' . $current_process_id;
  islandora_data_generate_csv_datastreams($derivs_source_filepath, $ext);
  $fh = fopen($csvlist_filepath, 'r');
  $worksheet_number = 0;
  while ($line = fgets($fh)) {
    $worksheet_number++;
    // @devel only: use an incrementor as the name of the worksheet. 
    // @todo: Get the worksheet name.
    dd($line, "Line");
    $file_basename = basename($line);
    // Add each datastream in the 'csvlist' file. Each one will be named after the 
    // worksheet from which it is derived, e.g. 'CSV_DATA_foo', 'CSV_DATA_bar', etc.
    islandora_data_add_datastream($object, 'CSV_DATA_' . $worksheet_number, trim($line));
  }
  fclose($fh);
  // During development, don't delete these files. In production, delete them.
  // file_delete($csvlist_filepath);
  // file_unmanaged_delete($derivs_source_file);
}

/**
 * Extract the CSV content from the uploaded file. Each worksheet in the
 * Excel or OpenOffice file will have one .csv DS.
 *
 * @param string $file_uri
 *   The Drupal URI of a copy of the OBJ file to extract CSV content from.
 * @param string $ext
 *   The extension of the file.
 *
 * @return string $output_file_path
 *   The absolute path to the output file.
 */
function islandora_data_generate_csv_datastreams($derivs_filepath, $ext) {
  $output = array();
  $input_file_path = drupal_realpath($derivs_filepath);
  $output_file_path = $input_file_path . '.csv';
  $current_process_id = getmypid();
  
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
      // $command = 'python /home/mark/Documents/apache_thinkpad/drupal7/sites/default/modules/islandora_solution_pack_data/csvnormalize.py ' . $input_file_path . ' ' . $current_process_id;
      $command = 'python /home/mark/Documents/apache_thinkpad/drupal7/sites/default/modules/islandora_solution_pack_data/csvnormalize.py ' . '/tmp/test_ds.csv' . ' ' . $current_process_id;
      dd($command, 'Command');
      exec($command, $output, $ret);
    case 'csv':
      file_put_contents('/tmp/csvlist.' . $current_process_id, $input_file_path . "\n");
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
  dd($dsid, 'DSID');
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $dsid;
    // Get the mime type of the file (should be text/csv).
    $mime_detect = new MimeDetect();
    $ds->mimeType = $mime_detect->getMimetype($file_uri);
    $ds->setContentFromFile($file_uri);
    $object->ingestDatastream($ds);
  }
  catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
  }
}


