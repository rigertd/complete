<?php
require_once('dbinfo.php');

include 'dbfuncs.php';

session_save_path('../../../temp/cs340');
session_start();

/* Check if user is logged in */
if (session_status() != PHP_SESSION_ACTIVE || !isset($_SESSION['username'])){
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no username or session--redirect to login.php */
    header("Location: https://{$host}{$url}/login.php");
    die();
}

/* get user info */
$username = $_SESSION['username'];
$user_id = $_SESSION['user_id'];
$first = $_SESSION['first'];
$ui_lang_id = $_SESSION['ui_lang_id'];
$ui_lang_name = $_SESSION['ui_lang_name'];

/* Establish database connection */
$mysqli = new mysqli(DB_ADDR, DB_USER, DB_PW, DB_NAME);
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

$profiles = getProfiles($mysqli, $email);


?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - User Dashboard</title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
</head>
<body>
<header>
    <nav class="top-bar" data-topbar role="navigation" data-options="is_hover: false">
        <ul class="title-area">
            <li class="name">
                <h1><a href="index.php">Issue Tracker</a></h1>
            </li>
            <li class="toggle-topbar menu-icon">
                <a href="#"><span></span></a>
            </li>
        </ul>
        <section class="top-bar-section">
            <ul class="right">
                <li class="divider"></li>
                <li><a href="profile.php"><?php echo htmlspecialchars($first); ?></a></li>
                <li class="divider"></li>
                <li class="has-dropdown">
                    <a href="#"><?php echo htmlspecialchars($ui_lang_name); ?></a>
                    <ul class="dropdown">
                        <li><a href="#">Español (España)</a></li>
                        <li><a href="#">日本語</a></li>
                    </ul>
                </li>
                <li class="divider"></li>
                <li class="has-form"><a href="login.php?action=logout" class="button small radius">Sign Out</a></li>
            </ul>
        </section>
    </nav>
</header>
<div class="medium-8 columns">
    <h3>User Dashboard</h3>
    <h4>Assigned Issues</h4>
    <table>
        <thead>
        <tr>
            <th>Issue #</th>
            <th>Subject</th>
            <th>Status</th>
            <th>Priority</th>
            <th>Due Date</th>
        </tr>
        </thead>
        <tbody>
        <tr>
            <td><a href="issue.php?id=1">1</a></td>
            <td><a href="issue.php?id=1">Issue Number 1</a></td>
            <td>In Progress</td>
            <td>High</td>
            <td>2015/07/26</td>
        </tr>
        </tbody>
    </table>
</div>
<div class="medium-4 columns panel">
    <ul class="side-nav" role="navigation" title="Project List">
        <li><h4>Projects</h4></li>
        <li role="menuitem"><a href="project.php?id=1">Project A</a></li>
        <li role="menuitem"><a href="project.php?id=2">Project B</a></li>
    </ul>
</div>
<div class="medium-12 columns text-center">&copy; David Rigert</div>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation/foundation.js"></script>
<script src="../js/foundation/foundation.topbar.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>