<?php
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $ethercalc_host.'/_');
curl_setopt($ch, CURLOPT_POST, 1);
curl_setopt($ch, CURLOPT_POSTFIELDS, $ds_content_file_path);
curl_setopt($ch, CURLOPT_HEADER, 1);
curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: text/csv"));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$output = curl_exec($ch); 
curl_close($ch);
?>