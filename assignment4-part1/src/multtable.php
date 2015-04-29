<?php
    $minMultiplicand = $_GET['min-multiplicand'];
    $maxMultiplicand = $_GET['max-multiplicand'];
    $minMultiplier = $_GET['min-multiplier'];
    $maxMultiplier = $_GET['max-multiplier'];
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Multiplication Table</title>
  </head>
  <body>
<?php if ($minMultiplicand > $maxMultiplicand): ?>
        <p>Minimum multiplicand larger than maximum.</p>
<?php elseif ($minMultiplier > $maxMultiplier): ?>
        <p>Minimum multiplier larger than maximum.</p>
<?php endif ?>
  </body>
</html>