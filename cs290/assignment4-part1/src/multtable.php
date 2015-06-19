<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
header('Content-Type: text/html');

$errorCount = 0; /* for keeping track of failed preconditions */
$bodyText = "";  /* for storing the contents of the body element */
/* check for preconditions */
if (!isset($_GET['min-multiplicand'])) {
    $bodyText .= "<p>Missing parameter [min-multiplicand].\n";
    $errorCount++;
} else {
    $minMultiplicand = $_GET['min-multiplicand'];
}
if (!isset($_GET['max-multiplicand'])) {
    $bodyText .= "<p>Missing parameter [max-multiplicand].\n";
    $errorCount++;
} else {
    $maxMultiplicand = $_GET['max-multiplicand'];
}
if (!isset($_GET['min-multiplier'])) {
    $bodyText .= "<p>Missing parameter [min-multiplier].\n";
    $errorCount++;
} else {
    $minMultiplier = $_GET['min-multiplier'];
}
if (!isset($_GET['max-multiplier'])) {
    $bodyText .= "<p>Missing parameter [max-multiplier].\n";
    $errorCount++;
} else {
    $maxMultiplier = $_GET['max-multiplier'];
}

/* make sure each value is a non-negative integer (contains only digits) */
if ($errorCount === 0) {
    if (!ctype_digit($minMultiplicand)) {
        $bodyText .= "<p>min-multiplicand must be a non-negative integer.\n";
        $errorCount++;
    }
    if (!ctype_digit($maxMultiplicand)) {
        $bodyText .= "<p>max-multiplicand must be a non-negative integer.\n";
        $errorCount++;
    }
    if (!ctype_digit($minMultiplier)) {
        $bodyText .= "<p>min-multiplier must be a non-negative integer.\n";
        $errorCount++;
    }
    if (!ctype_digit($maxMultiplier)) {
        $bodyText .= "<p>max-multiplier must be a non-negative integer.\n";
        $errorCount++;
    }
}

/* make sure minimum values are less than or equal to maximum values */
if ($errorCount === 0) {
    if ($errorCount === 0 && $minMultiplicand > $maxMultiplicand) {
        $bodyText .= "<p>Minimum multiplicand larger than maximum.\n";
        $errorCount++;
    }
    if ($errorCount === 0 && $minMultiplier > $maxMultiplier) {
        $bodyText .= "<p>Minimum multiplier larger than maximum.\n";
        $errorCount++;
    }
}

if ($errorCount === 0) {
    /* all preconditions are met--generate the multiplication table */
    $bodyText .= "<h2>Multiplication Table</h2>\n";
    $bodyText .= "<table>";
    for ($i = $minMultiplicand - 1; $i <= $maxMultiplicand; $i++) {
        $bodyText .= "\n<tr>";
        for ($j = $minMultiplier - 1; $j <= $maxMultiplier; $j++) {
            $bodyText .= "";
            if ($i == ($minMultiplicand - 1) && $j == ($minMultiplier - 1)) {
                /* top-left cell--leave blank */
                $bodyText .= "<th>&nbsp;";
            } elseif ($i == ($minMultiplicand - 1) && $j >= $minMultiplier) {
                /* top row--multipliers */
                $bodyText .= "<th>$j";
            } elseif ($i >= $minMultiplicand && $j == ($minMultiplier - 1)) {
                /* left column--multiplicands */
                $bodyText .= "<th>$i";
            } else {
                /* products */
                $bodyText .= "<td>" . ($i * $j);
            }
        }
    }
    $bodyText .= "\n</table>\n";
}
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Multiplication Table</title>
    <link rel="stylesheet" href="../style.css">
  </head>
  <body>
    <?php echo $bodyText ?>
  </body>
</html>