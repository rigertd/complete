<?php
$username = null;

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
  /* check for username parameter */
  if (array_key_exists('username', $_POST)) {
    $username = $_POST['username'];
  }
  if (empty($username)) {
    /* not found--display error message and die */
    echo <<< "LINE"
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Username Not Found</title>
  </head>
  <body>
    <p>A username must be entered. Click <a href="login.php">here</a> to return to the login screen.
  </body>
</html>
LINE;
    exit(1);
  }
} elseif (isset($_SESSION) && isset($_SESSION['count'])) {
  /* reuse existing session */
} else {
  /* no username or session--redirect to login.php */
  header("Location: login.php");
}
?>