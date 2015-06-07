<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'dbfuncs.php';

/* Redirect to HTTPS if using HTTP.
   Code taken from http://stackoverflow.com/questions/5106313/ */
if(!isset($_SERVER['HTTPS']) || $_SERVER['HTTPS'] == ""){
    $redirect = "https://".$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
    header("HTTP/1.1 301 Moved Permanently");
    header("Location: $redirect");
    die();
}

session_start();

/* for building URL to current path */
$host = $_SERVER['HTTP_HOST'];
$url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
$validation_message = '';

/* Establish database connection */
$mysqli = new mysqli(DB_ADDR, DB_USER, DB_PW, DB_NAME);
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/**
 * Determines whether an email address is already in use.
 * @param mysqli $db    The mysqli database interface.
 * @param String $email The e-mail address to check.
 * @return bool Whether the e-mail address is in use.
 */
function isEmailInUse($db, $email) {
    $stmt = prepareQuery($db, "SELECT COUNT(*) FROM Users WHERE email = ?");
    bindParam($stmt, "s", $email);
    executeStatement($stmt);
    $result = getSingleResult($stmt);
    return ($result["COUNT(*)"] == 0 ? false : true);
}

/**
 * Determines whether the specified email and password are correct.
 * Sets the session info on success.
 * @param mysqli $db    The mysqli database interface.
 * @param String $email The e-mail address.
 * @param String $pw    The plaintext password.
 * @return bool Whether authentication was successful
 */
function authenticate($db, $email, $pw) {
    /* query user database with login credentials */
    $hashed = hash('sha256', $pw);
    $stmt = prepareQuery($db, "SELECT COUNT(*), email, first_name, last_name FROM Users WHERE email = ? AND pass_hash = ?");
    bindParam($stmt, "ss", $email, $hashed);
    executeStatement($stmt);
    $result = getSingleResult($stmt);
    /* write user info to session if successful */
    if ($result["COUNT(*)"] > 0) {
        /* valid credentials--save to session */
        $_SESSION['email'] = $result['email'];
        $_SESSION['first'] = $result['first_name'];
        $_SESSION['last'] = $result['last_name'];
        return true;
    } else {
        /* invalid credentials */
        return false;
    }
}

/**
 * Adds a user with the specified information to the database.
 * @param mysqli $db    The mysqli database interface.
 * @param String $email The user's e-mail address.
 * @param String $first The user's first name.
 * @param String $last  The user's last name.
 * @param String $pw    The plaintext password.
 */
function addUser($db, $email, $first, $last, $pw) {
    $hashed = hash('sha256', $pw);
    $stmt = prepareQuery($db, "INSERT INTO Users (email, first_name, last_name, pass_hash) VALUES (?, ?, ?, ?)");
    bindParam($stmt, "ssss", $email, $first, $last, $hashed);
    executeStatement($stmt);
}

/* delete session data if logout action */
if (isset($_GET['action'])) {
    if ($_GET['action'] === 'logout') {
        if (session_status() == PHP_SESSION_ACTIVE) {
            session_destroy();
        }
        /* close DB connection on logout */
        $mysqli->close();
        /* redirect without logout action to avoid accidental logout */
        header("Location: https://{$host}{$url}/login.php");
        die();
    } else if ($_GET['action'] === 'email') {
        /* set content type to plain text */
        header('Content-Type: text/plain');
        /* check if email is already in use */
        $email = $_GET['signup_email'];
        echo isEmailInUse($mysqli, $email) ? 'true' : 'false';
        die();
    }
} else if (isset($_POST['action'])) {
    if ($_POST['action'] === 'auth') {
        /* set content type to plain text */
        header('Content-Type: text/plain');
        /* For authentication ajax requests.
           Returns 'true' if auth succeeds, 'false' otherwise. */
        $email = $_POST['email'];
        $pass = $_POST['password'];
        echo authenticate($mysqli, $email, $pass) ? 'true' : 'false';
        die();
    } else if ($_POST['action'] === 'signin') {
        /* For HTTP POST sign-in requests.
           Redirects to index.php or callback URL if auth succeeds. */
        $email = $_POST['email'];
        $pass = $_POST['password'];
        $callback = isset($_REQUEST['callback']) ?
            "https://{$host}{$_REQUEST['callback']}" :
            "https://{$host}{$url}/index.php";
        if (authenticate($mysqli, $email, $pass)) {
            header("Location: $callback");
            die();
        }
    } else if ($_POST['action'] === 'signup') {
        $email = $_POST['signup_email'];
        $first = $_POST['signup_first'];
        $last = $_POST['signup_last'];
        $pw1 = $_POST['signup_password1'];
        $pw2 = $_POST['signup_password2'];

        /* make sure required fields are filled in */
        if (empty($email) || empty($first) || empty($last) || empty($pw1)) {
            $validation_message .= '<p>Required information is missing or invalid.</p>';
        }
        /* make sure passwords match */
        if ($pw1 !== $pw2) {
            $validation_message .= '<p>The passwords do not match.</p>';
        }
        /* make sure email not already in use */
        if (isEmailInUse($mysqli, $email)) {
            $validation_message .= '<p>That e-mail address is already in use.</p>';
        } else if (strlen($validation_message) == 0) {
            /* add the user info to the database */
            addUser($mysqli, $email, $first, $last, $pw1);
            /* authenticate and login the user */
            if (authenticate($mysqli, $email, $pw1)) {
                /* account successfully created--redirect to index.php */
                header("Location: https://{$host}{$url}/index.php");
                die();
            }
            else {
                $validation_message = "<p>Sorry, we are unable to create an account for you at this time.</p>";
            }
        }
    }
} else if (session_status() == PHP_SESSION_ACTIVE && isset($_SESSION['email'])) {
    /* user already logged in--redirect to index.php */
    header("Location: https://{$host}{$url}/index.php");
    die();
}

header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="x-ua-compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Baby Growth Tracker</title>
    <link href="../css/bootstrap.min.css" rel="stylesheet">
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/tracker.min.js"></script>
    <script src="../js/login.min.js"></script>
  </head>
  <body>
    <div class="container">
      <header class="page-header clearfix">
        <div class="col-sm-6">
          <h2>Baby Growth Tracker</h2>
        </div>
        <div class="col-sm-6">
          <form id="login" method="post" action="login.php<?php echo (isset($_REQUEST['callback']) ? "?callback=$_REQUEST[callback]" : ""); ?>" style="padding-right:15px;" class="form-horizontal" role="form">
            <input type="hidden" name="action" value="signin">
            <div class="form-group">
              <div style="display:table-cell; padding-left:1em;">
                <label for="email" class="control-label">E-mail</label>
                <input id="email" type="email" class="form-control" name="email" placeholder="name@email.com" required>
              </div>
              <div style="display:table-cell; padding-left:1em;">
                <label for="password" class="control-label">Password</label>
                <input id="password" type="password" class="form-control" name="password" placeholder="Password" required>
              </div>
              <div style="display:table-cell; padding-left:1em; vertical-align: bottom;">
                <button type="submit" id="login_button" class="btn btn-default">Sign In</button>
              </div>
            </div>
          </form>
        </div>
      </header>
      <div class="jumbotron">
        <form id="signup" method="post" action="login.php" class="form-horizontal" role="form">
          <input type="hidden" name="action" value="signup">
          <h2>Create an Account</h2>
          <div class="form-group">
            <label for="signup_first" class="col-sm-2 control-label">First Name</label>
            <div class="col-sm-10">
              <input type="text" id="signup_first" title="First Name" class="form-control" name="signup_first" required>
            </div>
          </div>
          <div class="form-group">
            <label for="signup_last" class="col-sm-2 control-label">Last Name</label>
            <div class="col-sm-10">
              <input type="text" id="signup_last" title="Last Name" class="col-sm-10 form-control" name="signup_last" required>
            </div>
          </div>
          <div class="form-group">
            <label for="signup_email" class="col-sm-2 control-label">E-mail Address</label>
            <div class="col-sm-10">
              <input type="email" id="signup_email" title="E-mail Address" class="col-sm-10 form-control" name="signup_email" required>
            </div>
          </div>
          <div class="form-group">
            <label for="signup_password1" class="col-sm-2 control-label">Password</label>
            <div class="col-sm-10">
                <input type="password" id="signup_password1" title="Password" class="col-sm-10 form-control" name="signup_password1" required>
            </div>
          </div>
          <div class="form-group">
            <label for="signup_password2" class="col-sm-2 control-label">Confirm Password</label>
            <div class="col-sm-10">
                <input type="password" id="signup_password2" title="Confirm Password" class="col-sm-10 form-control" name="signup_password2" required>
            </div>
          </div>
          <div class="form-group">
            <div class="<?php echo (strlen($validation_message) > 0 ? 'alert alert-danger' : '') ?>" id="validation_message"><?php echo $validation_message ?></div>
          </div>
          <div class="form-group">
            <button type="submit" id="signup_button" class="btn btn-default center-block">Sign Up</button>
          </div>
        </form>
      </div>
      <footer>
        <p>&copy; David Rigert</p>
      </footer>
    </div>
  </body>
</html>