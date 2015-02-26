<?php

/**
 * @file
 * This is the template file for the object page for basic image
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-basic-image-object islandora">
  <div class="islandora-basic-image-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="message">
        <?php print $islandora_content;
        print "\n";
        exec('curl -i -H "Content-Type: text/csv" -X POST --data-binary @'.$islandora_content.' http://localhost:8000/');
        print "\n";
        ?>
      </div>
    <?php endif; ?>
  <div class="islandora-basic-image-sidebar">
    <?php if (!empty($dc_array['dc:description']['value'])): ?>
      <h2><?php print $dc_array['dc:description']['label']; ?></h2>
      <p><?php print str_replace("\n","<br>",$dc_array['dc:description']['value']); ?></p>
    <?php endif; ?>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
  </div>
  <div>
  <h2>Cite as</h2>
  <p>
  <?php print_r($dc_array["dc:creator"]["value"]);
  print " (";
  print_r(substr($dc_array["dc:date"]["value"], 0, 4));
  print "): ";
  print_r($dc_array["dc:title"]["value"]);
  print ". http://researchdata.sfu.ca/islandora/object/";
  print_r($dc_array["dc:identifier"]["value"]);
  print "."; ?>
  </p>
  </div>
  <?php print $metadata; ?>
  </div>
</div>
