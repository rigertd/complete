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
    /* user already logged in--redirect to index.php */
    header("Location: http://{$host}{$url}/index.php");
}
header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="x-ua-compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Baby Growth Tracker Sign-in</title>
    <link href="../css/bootstrap.min.css" rel="stylesheet">
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
    <link href="../css/style.css" rel="stylesheet">
  </head>
  <body>
    <header class="page-header">
      <form id="login" class="form-inline pull-right" role="form">
        <div class="form-group">
          <label for="email" class="control-label">E-mail</label>
          <input id="email" type="email" class="form-control" name="email" placeholder="E-mail address" required>
        </div>
        <div class="form-group">
          <label for="password" class="control-label">Password</label>
          <input id="password" type="password" class="form-control" name="password" placeholder="Password" required>
        </div>
        <div class="form-group">
          <button type="submit" id="login_button" class="btn btn-default">Sign In</button>
        </div>
      </form>
      <h1>Baby Growth Tracker</h1>
    </header>
    <div class="container">
      <form id="signup" class="form-horizontal" role="form">
        <div class="jumbotron">
          <h2>Sign Up</h2>
          <label for="signup_first" class="control-label">First Name</label>
          <input type="text" id="signup_first" class="form-control" name="signup_first" required>
          <label for="signup_last" class="control-label">Last Name</label>
          <input type="text" id="signup_last" class="form-control" name="signup_last" required>
          <label for="signup_email" class="control-label">E-mail Address</label>
          <input type="email" id="signup_email" class="form-control" name="signup_email" required>
          <label for="signup_password1" class="control-label">Password</label>
          <input type="password" id="signup_password1" class="form-control" name="signup_password1" required>
          <label for="signup_password2" class="control-label">Confirm Password</label>
          <input type="password" id="signup_password2" class="form-control" name="signup_password2" required>
          <div class="form-group">
            <button type="submit" id="signup_button" class="btn btn-default">Create Account</button>
          </div>
        </div>
      </form>
      <footer>
        <p>&copy; David Rigert</p>
      </footer>
    </div>
  </body>
</html>