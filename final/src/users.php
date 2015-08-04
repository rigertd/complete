<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* language administration functions */
function addUser($db, $first_name, $last_name, $username, $email, $lang_id, $pw) {
    $hashed = hash('sha256', $pw);
    $stmt = prepareQuery($db, "INSERT INTO Users (first_name, last_name, username, email, ui_lang_id, pass_hash) VALUES (?, ?, ?, ?, ?, ?);");
    bindParam($stmt, "ssssis", $first_name, $last_name, $username, $email, $lang_id, $hashed);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['users']);
        return true;
    }
    return false;
}

function updateUser($db, $user_id, $first_name, $last_name, $username, $email, $lang_id, $pw) {
    if ($last_name == '') {
        $last_name = NULL;
    }
    if ($email == '') {
        $email = NULL;
    }
    if ($pw == NULL || $pw == '') {
        $stmt = prepareQuery($db, "UPDATE Users SET first_name = ?, last_name = ?, username = ?, email = ?, ui_lang_id = ? WHERE user_id = ?;");
        bindParam($stmt, "ssssii", $first_name, $last_name, $username, $email, $lang_id, $user_id);
    } else {
        $hashed = hash('sha256', $pw);
        $stmt = prepareQuery($db, "UPDATE Users SET first_name = ?, last_name = ?, username = ?, email = ?, ui_lang_id = ?, pass_hash = ? WHERE user_id = ?;");
        bindParam($stmt, "ssssisi", $first_name, $last_name, $username, $email, $lang_id, $hashed, $user_id);
    }
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['users']);
        return true;
    }
    return false;
}

function deleteUser($db, $user_id) {
    $stmt = prepareQuery($db, "DELETE FROM Users WHERE user_id = ?;");
    bindParam($stmt, "i", $user_id);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['users']);
        return true;
    }
    return false;
}


/* check for URL params */
if (!isAdmin($mysqli, $user_id)) {
    /* redirect to dashboard if not admin */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/index.php");
    die();
} else if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
} else if (isset($_REQUEST['action'])) {
    $_SESSION['alert'] = '<script>';
    if ($_REQUEST['action'] == 'update') {
        if (updateUser($mysqli, $_POST['user_id'], $_POST['first_name'], $_POST['last_name'], $_POST['username'], $_POST['email'], $_POST['ui_lang_id'], $_POST['password'])) {
            $_SESSION['alert'] .= "alert('$_POST[username] was updated.');";
        } else {
            $_SESSION['alert'] .= "alert('Update of $_POST[username] failed.');";
        }
    } else if ($_REQUEST['action'] == 'new') {
        if (addUser($mysqli, $_POST['first_name'], $_POST['last_name'], $_POST['username'], $_POST['email'], $_POST['ui_lang_id'], $_POST['password'])) {
            $_SESSION['alert'] .= "alert('$_POST[username] was added.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to add $_POST[username]. Make sure the username does not already exist.');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deleteUser($mysqli, $_POST['user_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[username] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[username].');";
        }
    } else {
        $_SESSION['alert'] .= "alert('Invalid action specified.');";
    }
    $_SESSION['alert'] .= '</script>';

    /* redirect to remove URL parameters */
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    $url = strtok($url, '?');
    header("Location: https://$url");
    die();
}

/* get UI languages */
$languages = getLanguages($mysqli);

/* get user list */
$users = getAllUsers($mysqli);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Admin - Users</title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
    <script>
        window.onload = function() {
            var dels = document.getElementsByName('action');
            for (var i = 0; i < dels.length; i++) {
                if (dels[i].value == 'delete') {
                    dels[i].onclick = function() {
                        return confirm('Are you sure? This cannot be undone.');
                    };
                }
            }
        };
    </script>
</head>
<body>
<?php echo $alert ?>
<?php include('nav.php'); ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="unavailable">Admin</li>
    <li class="current">Users</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>All Users</h3></li>
    </ul>
    <div class="clearfix">
        <div class="medium-1 column"><strong>ID</strong></div>
        <div class="medium-1 column"><strong>Username</strong></div>
        <div class="medium-2 column"><strong>First</strong></div>
        <div class="medium-1 column"><strong>Last</strong></div>
        <div class="medium-2 column"><strong>Email</strong></div>
        <div class="medium-2 column"><strong>UI Language</strong></div>
        <div class="medium-1 column"><strong>Password</strong></div>
        <div class="medium-2 column">&nbsp;</div>
    </div>
<?php if (count($users) > 0): ?>
<?php foreach ($users as $user): ?>
    <div class="clearfix">
        <form method="POST" action="users.php" data-abide>
            <div class="medium-1 column"><?php echo $user['user_id']; ?><input type="hidden" name="user_id" value="<?php echo $user['user_id']; ?>"></div>
            <div class="medium-1 column">
                <input type="text" name="username" value="<?php echo htmlspecialchars($user['username']); ?>" required pattern="[-_a-zA-Z\d]+">
                <small class="error">A unique username is required.</small>
            </div>
            <div class="medium-2 column">
                <input type="text" name="first_name" value="<?php echo htmlspecialchars($user['first_name']); ?>" required>
                <small class="error">A first name is required.</small>
            </div>
            <div class="medium-1 column"><input type="text" name="last_name" value="<?php echo htmlspecialchars($user['last_name']); ?>"></div>
            <div class="medium-2 column">
                <input type="email" name="email" value="<?php echo htmlspecialchars($user['email']); ?>">
                <small class="error">Enter a valid email address or leave it blank to clear.</small>
            </div>
            <div class="medium-2 column">
                <select name="ui_lang_id" required>
<?php foreach ($languages as $lang): ?>
                    <option value="<?php echo $lang['lang_id']; ?>"<?php echo ($lang['lang_id'] == $user['ui_lang_id'] ? 'selected' : ''); ?>><?php echo htmlspecialchars($lang['name']); ?></option>
<?php endforeach ?>
                </select>
                <small class="error">A UI language is required.</small>
            </div>
            <div class="medium-1 column"><input type="password" name="password" placeholder="●●●●●●●●"></div>
            <div class="medium-2 column">
                <ul class="button-group radius">
                    <li><button type="submit" name="action" value="update" title="Save" class="button tiny"><i class="fi-save" style="font-size: 150%"></i></button></li>
                    <li><button class="button tiny" type="submit" name="action" value="delete" title="Delete"><i class="fi-trash" style="font-size: 150%"></i></button></li>
                </ul>
            </div>
        </form>
    </div>
<?php endforeach ?>
<?php else: ?>
    <div class="clearfix">
        <div class="small-12 column">There are no users registered.</div>
    </div>
<?php endif ?>
    <div class="clearfix">
        <hr>
        <form method="POST" action="users.php" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-1 column">
                <input type="text" name="username" placeholder="Username" required>
                <small class="error">A unique username is required.</small>
            </div>
            <div class="medium-2 column">
                <input type="text" name="first_name" placeholder="First name" required>
                <small class="error">A first name is required.</small>
            </div>
            <div class="medium-1 column"><input type="text" name="last_name" placeholder="Last name (optional)"></div>
            <div class="medium-2 column">
                <input type="email" name="email" placeholder="Email (optional)">
                <small class="error">Enter a valid email address or leave it blank.</small>
            </div>
            <div class="medium-2 column">
                <select name="ui_lang_id" required>
<?php foreach ($languages as $lang): ?>
                        <option value="<?php echo $lang['lang_id']; ?>"<?php echo ($lang['lang_id'] == $ui_lang_id ? 'selected' : ''); ?>><?php echo htmlspecialchars($lang['name']); ?></option>
<?php endforeach ?>
                </select>
                <small class="error">A UI language is required.</small>
            </div>
            <div class="medium-1 column">
                <input type="password" name="password" placeholder="Password" required>
                <small class="error">A password must be set for new users.</small>
            </div>
            <div class="medium-2 column">
                <ul class="button-group radius">
                    <li><button type="submit" title="Add" name="action" value="new" class="button tiny"><i class="fi-plus" style="font-size: 150%"></i></button></li>
                </ul>
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