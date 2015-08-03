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

session_save_path('../../../temp/cs340');
session_start();

/* for building URL to current path */
$host = $_SERVER['HTTP_HOST'];
$url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
$validation_message = '';

/* Establish database connection */
$mysqli = new mysqli(DB_ADDR, DB_USER, DB_PW, DB_NAME);
$mysqli->set_charset('utf8');
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/**
 * Determines whether the specified email and password are correct.
 * Sets the session info on success.
 * @param mysqli $db    The mysqli database interface.
 * @param String $un    The username.
 * @param String $pw    The plaintext password.
 * @return bool Whether authentication was successful
 */
function authenticate($db, $un, $pw) {
    /* query user database with login credentials */
    $hashed = hash('sha256', $pw);
    $stmt = prepareQuery($db, "SELECT COUNT(*), user_id, username, first_name, ui_lang_id, Languages.name AS ui_lang_name FROM Users INNER JOIN Languages ON Users.ui_lang_id = Languages.lang_id WHERE username = ? AND pass_hash = ?");
    bindParam($stmt, "ss", $un, $hashed);
    executeStatement($stmt);
    $result = getSingleResult($stmt);
    /* write user info to session if successful */
    if ($result["COUNT(*)"] > 0) {
        /* valid credentials--save to session */
        $_SESSION['username'] = $result['username'];
        $_SESSION['user_id'] = $result['user_id'];
        $_SESSION['first'] = $result['first_name'];
        $_SESSION['ui_lang_id'] = $result['ui_lang_id'];
        $_SESSION['ui_lang_name'] = $result['ui_lang_name'];
        /* get cookie info (if any) */
        if (isset($_COOKIE['issue_sort_col'])) {
            $_SESSION['issue_sort_col'] = $_COOKIE['issue_sort_col'];
        } else {
            setcookie("issue_sort_col","1",time()+60*60*24*30);
            $_SESSION['issue_sort_col'] = "1";
        }
        if (isset($_COOKIE['issue_sort_dir'])) {
            $_SESSION['issue_sort_dir'] = $_COOKIE['issue_sort_dir'];
        } else {
            setcookie("issue_sort_dir","ASC",time()+60*60*24*30);
            $_SESSION['issue_sort_dir'] = "ASC";
        }
        if (isset($_COOKIE['proj_sort_col'])) {
            $_SESSION['proj_sort_col'] = $_COOKIE['proj_sort_col'];
        } else {
            setcookie("proj_sort_col","1",time()+60*60*24*30);
            $_SESSION['proj_sort_col'] = "1";
        }
        if (isset($_COOKIE['proj_sort_dir'])) {
            $_SESSION['proj_sort_dir'] = $_COOKIE['issue_sort_dir'];
        } else {
            setcookie("proj_sort_dir","ASC",time()+60*60*24*30);
            $_SESSION['proj_sort_dir'] = "ASC";
        }
        return true;
    } else {
        /* invalid credentials */
        return false;
    }
}

/* delete session data if logout action */
if (isset($_REQUEST['action'])) {
    if ($_REQUEST['action'] === 'logout') {
        if (session_status() == PHP_SESSION_ACTIVE) {
            session_destroy();
        }
        /* close DB connection on logout */
        $mysqli->close();
        /* redirect without logout action to avoid accidental logout */
        header("Location: https://{$host}{$url}/login.php");
        die();
    }
} else if (session_status() == PHP_SESSION_ACTIVE && isset($_SESSION['username'])) {
    /* user already logged in--redirect to index.php */
    header("Location: https://{$host}{$url}/index.php");
    die();
} else {
    if (isset($_REQUEST['username'])) {
        /* For HTTP POST sign-in requests.
           Redirects to index.php or callback URL if auth succeeds. */
        $username = $_REQUEST['username'];
        $pass = $_REQUEST['password'];
        $callback = isset($_REQUEST['callback']) ?
            "https://{$host}{$_REQUEST['callback']}" :
            "https://{$host}{$url}/index.php";
        if (authenticate($mysqli, $username, $pass)) {
            header("Location: $callback");
            die();
        } else {
            $validation_message = "Invalid login credentials.";
        }
    }
}

header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Sign-in</title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
  </head>
  <body>
    <header>
      <nav class="top-bar">
        <ul class="title-area">
          <li class="name">
            <h1><a href="#">Issue Tracker</a></h1>
          </li>
        </ul>
      </nav>
    </header>
    <div class="row">
      <div class="medium-12 columns">
        <div class="panel">
          <h3>Enter your username and password</h3>
<?php if ($validation_message !== ''): ?>
          <div class="alert-box alert radius">
            <?php echo $validation_message ?>
          </div>
<?php endif ?>
          <form method="POST" action="login.php" data-abide>
            <div class="row">
              <div class="medium-6 columns">
                <label for="username">Username</label>
                <input type="text" id="username" name="username" placeholder="Username" required>
                <small class="error">You must enter a username.</small>
              </div>
              <div class="medium-6 columns">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" placeholder="Password" required>
                <small class="error">You must enter a password.</small>
              </div>
            </div>
            <div class="row">
              <div class="medium-12 columns">
                <button type="submit" class="small radius button">Sign In</button>
              </div>
            </div>
          </form>
        </div>
      </div>
    </div>
    <footer class="row">
      <div class="small-12 columns text-center">&copy; David Rigert</div>
    </footer>
    <script src="../js/vendor/jquery.js"></script>
    <script src="../js/foundation/foundation.js"></script>
    <script src="../js/foundation/foundation.abide.js"></script>
    <script>
        $(document).foundation();
    </script>
  </body>
</html>