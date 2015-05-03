<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

session_start();

$title = "";        // stores output page title
$message = "";      // stores output page text
$count = 0;         // stores number of page visits

/* redirect to login.php if username is not set */
if (session_status() != PHP_SESSION_ACTIVE || !isset($_SESSION['username'])) {
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no session username--redirect to login.php */
    header("Location: http://{$host}{$url}/login.php");
    die();
}

$title = "Page Two";
$message = <<<"MSG"
<p>This is your second page of special content.
<p>Go back to the <a href="content1.php">first page</a>.

MSG;

header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title><?php echo $title ?></title>
</head>
<body>
<p><?php echo $message ?>
</body>
</html>
