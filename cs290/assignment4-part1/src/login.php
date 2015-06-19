<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

session_start();

/* for building URL to current path */
$host = $_SERVER['HTTP_HOST'];
$url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");

/* delete session data if logout action */
if (isset($_GET) && isset($_GET['action']) && $_GET['action'] === 'logout') {
    if (session_status() == PHP_SESSION_ACTIVE) {
        session_destroy();
    }
    /* redirect without logout action to avoid accidental logout */
    header("Location: http://{$host}{$url}/login.php");
    die();
} else if (session_status() == PHP_SESSION_ACTIVE && isset($_SESSION['username'])) {
    /* user already logged in--redirect to content1.php */
    header("Location: http://{$host}{$url}/content1.php");
}
header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Login</title>
    <link rel="stylesheet" href="../style.css">
  </head>
  <body>
    <form action="content1.php" method="POST">
      <fieldset>
        <legend>User Login</legend>
        <label>Username: <input type="text" name="username"></label>
        <button type="submit">Login</button>
      </fieldset>
    </form>
  </body>
</html>