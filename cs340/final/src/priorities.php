<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* priority administration functions */
function addPriority($db, $name, $val) {
    $stmt = prepareQuery($db, "INSERT INTO Priorities (name, val) VALUES (?, ?);");
    bindParam($stmt, "si", $name, $val);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['priorities']);
        return true;
    }
    return false;
}

function updatePriority($db, $priorityId, $name, $val) {
    $stmt = prepareQuery($db, "UPDATE Priorities SET name = ?, val = ? WHERE priority_id = ?;");
    bindParam($stmt, "sii", $name, $val, $priorityId);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['priorities']);
        return true;
    }
    return false;
}

function deletePriority($db, $priorityId) {
    $stmt = prepareQuery($db, "DELETE FROM Priorities WHERE priority_id = ?;");
    bindParam($stmt, "i", $priorityId);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['priorities']);
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
        if (updatePriority($mysqli, $_POST['priority_id'], $_POST['priority_name'], $_POST['priority_val'])) {
            $_SESSION['alert'] .= "alert('$_POST[priority_name] was updated.');";
        } else {
            $_SESSION['alert'] .= "alert('Update of $_POST[priority_name] failed.');";
        }
    } else if ($_REQUEST['action'] == 'new') {
        if (addPriority($mysqli, $_POST['priority_name'], $_POST['priority_val'])) {
            $_SESSION['alert'] .= "alert('$_POST[priority_name] was added.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to add $_POST[priority_name]. Make sure it does not already exist.');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deletePriority($mysqli, $_POST['priority_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[priority_name] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[priority_name]. Make sure it is not used by any issues.');";
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
/* get priority list */
$priorities = getPriorities($mysqli);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Admin - Priorities</title>
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
    <li class="current">Priorities</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Issue Priorities</h3></li>
    </ul>
    <div class="clearfix">
        <div class="medium-1 column"><strong>ID</strong></div>
        <div class="medium-4 column"><strong>Name</strong></div>
        <div class="medium-4 column"><strong>Value</strong></div>
        <div class="medium-3 column">&nbsp;</div>
    </div>
<?php if (count($priorities) > 0): ?>
<?php foreach ($priorities as $prio): ?>
    <div class="clearfix">
        <form method="POST" action="priorities.php" data-abide>
            <div class="medium-1 column"><?php echo $prio['priority_id']; ?><input type="hidden" name="priority_id" value="<?php echo $prio['priority_id']; ?>"></div>
            <div class="medium-4 column">
                <input type="text" name="priority_name" value="<?php echo $prio['name']; ?>" required>
                <small class="error">A priority name is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="priority_val" value="<?php echo $prio['val']; ?>" pattern="\d+" required>
                <small class="error">A priority value is required.</small>
            </div>
            <div class="medium-3 column">
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
        <div class="small-12 column">There are no priorities registered.</div>
    </div>
<?php endif ?>
    <div class="clearfix">
        <hr>
        <form method="POST" action="priorities.php" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-4 column">
                <input type="text" name="priority_name" placeholder="Priority name" required>
                <small class="error">A priority name is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="priority_val" placeholder="Priority value (integer)" pattern="\d+" required>
                <small class="error">A priority value is required.</small>
            </div>
            <div class="medium-3 column">
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