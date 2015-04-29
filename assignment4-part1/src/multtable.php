<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Multiplication Table</title>
    <link rel="stylesheet" href="../style.css">
  </head>
  <body>
<?php
$errorCount = 0; /* for keeping track of failed preconditions */
/* check for preconditions */
if (!array_key_exists('min-multiplicand', $_GET)) {
    echo "<p>Missing parameter [min-multiplicand].</p>";
    $errorCount++;
}
if (!array_key_exists('max-multiplicand', $_GET)) {
    echo "<p>Missing parameter [max-multiplicand].</p>";
    $errorCount++;
}
if (!array_key_exists('min-multiplier', $_GET)) {
    echo "<p>Missing parameter [min-multiplier].</p>";
    $errorCount++;
}
if (!array_key_exists('max-multiplier', $_GET)) {
    echo "<p>Missing parameter [max-multiplier].</p>";
    $errorCount++;
}
/* only get values from request if all parameters exist */
if ($errorCount > 0) {
    exit(1);
} else {
    $minMultiplicand = $_GET['min-multiplicand'];
    $maxMultiplicand = $_GET['max-multiplicand'];
    $minMultiplier = $_GET['min-multiplier'];
    $maxMultiplier = $_GET['max-multiplier'];
}
/* make sure each value is an integer */
if (!ctype_digit($minMultiplicand)) {
    echo "<p>min-multiplicand is not an integer.</p>";
    $errorCount++;
}
if (!ctype_digit($maxMultiplicand)) {
    echo "<p>max-multiplicand is not an integer.</p>";
    $errorCount++;
}
if (!ctype_digit($minMultiplier)) {
    echo "<p>min-multiplier is not an integer.</p>";
    $errorCount++;
}
if (!ctype_digit($maxMultiplier)) {
    echo "<p>max-multiplier is not an integer.</p>";
    $errorCount++;
}
/* stop script if any variables are non-integers */
if ($errorCount > 0) {
    exit(1);
}
/* make sure minimum values are less than or equal to maximum values */
if ($minMultiplicand > $maxMultiplicand || $minMultiplier > $maxMultiplier) {
    if ($minMultiplicand > $maxMultiplicand)
        echo "<p>Minimum multiplicand larger than maximum.</p>";
    if ($minMultiplier > $maxMultiplier)
        echo "<p>Minimum multiplier larger than maximum.</p>";
    exit(1);
}

/* all preconditions are met--generate the multiplication table */
echo "<h2>Multiplication Table</h2>\n";
echo "<table>";
for ($i = $minMultiplicand - 1; $i <= $maxMultiplicand; $i++) {
    echo "\n<tr>";
    for ($j = $minMultiplier - 1; $j <= $maxMultiplier; $j++) {
        echo "";
        if ($i == ($minMultiplicand - 1) && $j == ($minMultiplier - 1)) {
            /* top-left cell--leave blank */
            echo "<th>&nbsp;";
        } elseif ($i == ($minMultiplicand - 1) && $j >= $minMultiplier) {
            /* top row--multipliers */
            echo "<th>$j";
        } elseif ($i >= $minMultiplicand && $j == ($minMultiplier - 1)) {
            /* left column--multiplicands */
            echo "<th>$i";
        } else {
            /* products */
            echo "<td>" . ($i * $j);
        }
    }
}
echo "\n</table>\n";
?>
  </body>
</html>