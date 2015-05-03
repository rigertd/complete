<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

session_start();

$username = "";     // stores username (if any)
$s_username = "";   // stores username in session (if any)
$title = "";        // stores output page title
$message = "";      // stores output page text
$count = 0;         // stores number of page visits

/* redirect to login.php if not a POST and there is no session */
if ($_SERVER['REQUEST_METHOD'] !== 'POST' && session_status() != PHP_SESSION_ACTIVE) {
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no username or session--redirect to login.php */
    header("Location: http://{$host}{$url}/login.php");
    die();
}

/* try to get username and count from session if one exists */
if (session_status() == PHP_SESSION_ACTIVE) {
    if (isset($_SESSION['username'])) {
        $s_username = $_SESSION['username'];
    }
    if (isset($_SESSION['count'])) {
        $count = $_SESSION['count'];
    }
}

/* try to get username if POST request */
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['username'])) {
    $username = $_POST['username'];
}

if (empty($username) && empty($s_username)) {
    /* both usernames are empty--display error message */
    $title = 'Username not found';
    $message .= <<<"MSG"
<p>A username must be entered. Click <a href="login.php">here</a> to return to the login screen.

MSG;

} else {
    if (!empty($s_username) && !empty($username) && $username !== $s_username) {
        /* different user logged in--destroy old session and create a new one */
        session_destroy();
        session_start();
        $count = 0;
        $message .= "<p>{$s_username} has been logged out.\n";
    }
    $username = (empty($username) ? $s_username : $username);
    $title = "Page One";
    $time_plural = ($count === 1 ? "time" : "times");
    $message .= <<<"MSG"
<p>Hello {$username}, you have visited this page $count $time_plural before.
<p>Click <a href="content2.php">here</a> to see page two of the content.
<p>Click <a href="login.php?action=logout">here</a> to logout.

MSG;
    /* update session info */
    $_SESSION['username'] = $username;
    $_SESSION['count'] = ++$count;
}
header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title><?php echo $title ?></title>
</head>
<body>
<?php echo $message ?>
</body>
</html>
