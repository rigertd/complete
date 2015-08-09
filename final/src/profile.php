<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/**
 * Gets information on the specified user.
 * @param $db
 * @param $user_id
 * @return mixed
 */
function getUserInfo($db, $user_id) {
    $result = $db->query("SELECT username, first_name, last_name, email FROM Users WHERE user_id = $user_id;");
    return $result->fetch_assoc();
}

/* get UI languages */
$languages = getLanguages($mysqli);

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
} else if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'update') {
    updateUser($mysqli, $user_id, $_REQUEST['first_name'], $_REQUEST['last_name'], $_SESSION['username'], $_REQUEST['email'], $ui_lang_id, $_REQUEST['pw']);
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/index.php");
    die();
}

$user = getUserInfo($mysqli, $user_id);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - User Profile for <?php echo htmlspecialchars($user['username']); ?></title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
</head>
<body>
<?php include('nav.php'); ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="current">User Profile</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom:0;">
        <li><h3>Update User Profile</h3></li>
    </ul>
    <div class="medium-12 panel">
        <form action="profile.php" method="POST" data-abide>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="first_name">First Name</label>
                </div>
                <div class="medium-4 column">
                    <input type="text" id="first_name" name="first_name" value="<?php echo htmlspecialchars($user['first_name']); ?>" required>
                    <small class="error">You must enter your first name.</small>
                </div>
                <div class="medium-2 column">
                    <label for="last_name">Last Name</label>
                </div>
                <div class="medium-4 column">
                    <input type="text" id="last_name" name="last_name" value="<?php echo htmlspecialchars($user['last_name']); ?>" placeholder="Last Name (Optional)">
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="email">Email Address</label>
                </div>
                <div class="medium-10 column">
                    <input type="email" id="email" name="email" value="<?php echo htmlspecialchars($user['email']); ?>" placeholder="Email Address (Optional)">
                    <small class="error">You must enter a valid email address or leave it blank.</small>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="password">Password</label>
                </div>
                <div class="medium-10 column">
                    <input type="password" id="password" name="pw" placeholder="●●●●●●●●">
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" name="action" value="update" class="button tiny">Save</button></li>
                        <li><a href="index.php" class="button tiny">Cancel</a></li>
                    </ul>
                </div>
            </div>
        </form>
    </div>
</div>
<div class="medium-12 columns text-center">&copy; David Rigert</div>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation/foundation.js"></script>
<script src="../js/foundation/foundation.topbar.js"></script>
<script src="../js/foundation/foundation.abide.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>
