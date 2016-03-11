<?php

/**
 * @file
 * Functions for creating derivatives of the Excel, SPSS and OpenOffice Calc files.
 * We do not create derivatives of CSV files.
 */

/**
 * Creates the derivatives for this content model type. In addition to a
 * TN datastream, for each worksheet in Excel and OpenOffice Calc files, a
 * CSV datastream will be created.
 *
 * @param AbstractObject $object
 *   The object to add derivatives to.
 */
function islandora_data_create_all_derivatives(AbstractObject $object) {
  $mime_detect = new MimeDetect();
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not normalize %s. No valid file was uploaded.',
      array('%s' => $object->id)), 'error');
    return "";
  }
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object for the uploaded file.
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

  // Add CSV datastream(s). There will be one per worksheet in the Excel
  // and OpenOffice Calc files.
  $derivs_source_filepath = file_unmanaged_copy($original_file->uri, 'temporary://' .
    $file_name . '.derivs.' . $ext, FILE_EXISTS_REPLACE);

  // The current process ID will be passed to csvnormalize.py as a parameter in
  // islandora_data_generate_csv_datastreams() so it knows what output file to
  // write to. Here, we grab the process ID so we can read the same file.
  $current_process_id = getmypid();
  $csvlist_filepath = '/tmp/csvlist.' . $current_process_id;

  islandora_data_generate_csv_datastreams($derivs_source_filepath, $ext);

  // Now that the .csv derivatives have been created, open the derivatives list
  // written to by csvnormalize.py and add the CSV datastream identifed in each
  // line of the file.
  $worksheet_number = 0;
  $fh = fopen($csvlist_filepath, 'r');
  while ($deriv_path = fgets($fh)) {
    $worksheet_number++;
    $file_basename = basename(trim($deriv_path));
    $dsid = 'CSV_DATA_' . $worksheet_number;
    // Each derivative's filename has 'csvlist.nnn.' prepended to it, which we
    // want to remove to use as the datastream label.
    $to_remove = 'csvlist.' . $current_process_id . '.';
    $ds_label= preg_replace('/' . $to_remove . '/', '', $file_basename);
    islandora_data_add_datastream($object, $dsid, $ds_label, trim($deriv_path));
  }
  fclose($fh);
  // @todo: During development, don't delete these files. In production, delete them.
  // file_unmanaged_delete($csvlist_filepath);
  // file_unmanaged_delete($derivs_source_filepath);
}

/**
 * Extract the CSV content from the uploaded file. Each worksheet in the
 * Excel or OpenOffice file will have one .csv datastream.
 *
 * @param string $deriv_source_file_path
 *   The Drupal URI of a copy of the OBJ file to extract CSV content from.
 * @param string $ext
 *   The extension of the file.
 */
function islandora_data_generate_csv_datastreams($deriv_source_file_path, $ext) {
  $output = array();
  $input_file_path = drupal_realpath($deriv_source_file_path);
  // Get the current process ID so we can ensure a unique filename for the
  // output of csvnormalize.py.
  $current_process_id = getmypid();

  // For spreadsheet files, geneate a .csv for each worksheet by calling
  // csvnormalize.py. For native .csv files, return the original file path,
  // in which case the .csv file itself will be used as the content of the
  // CSV_DATA datastream.
  switch ($ext) {
    case 'xls':
    case 'xlsx':
    case 'ods':
      // Extract the CSV content from files with these extensions.
      $path_to_csvnormalize_py = drupal_get_path('module', 'islandora_data');
      $command = 'python ' . $path_to_csvnormalize_py . '/csvnormalize.py ' .
        $input_file_path . ' ' . $current_process_id;
      exec($command, $output, $ret);
      break;
    case 'sav':
      // Run pspp-convert instead to directly convert SPSS to CSV
      $command = 'pspp-convert' . ' -O csv ' . $input_file_path . ' ' . '/tmp/csvlist.' . $current_process_id;
      exec($command, $output, $rest);
      break;
    case 'csv':
      // Skip csvnormalize.py and just write the path to the native .csv file to the derivatives list.
      file_put_contents('/tmp/csvlist.' . $current_process_id, $input_file_path . "\n");
      break;
  }
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $ds_label
 *   The datastream label to be added.
 * @param string $file_uri
 *   The URI (full Drupal path) of the file to be added as the datastream content.
 */
function islandora_data_add_datastream(AbstractObject $object, $dsid, $ds_label, $file_uri) {
  try {
    $ds = $object->constructDatastream($dsid, 'M');
    $ds->label = $ds_label;
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
