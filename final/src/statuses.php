<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* status administration functions */
function addStatus($db, $name, $percent) {
    $stmt = prepareQuery($db, "INSERT INTO Statuses (name, percentage) VALUES (?, ?);");
    bindParam($stmt, "si", $name, $percent);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['statuses']);
        return true;
    }
    return false;
}

function updateStatus($db, $status_id, $name, $percent) {
    $stmt = prepareQuery($db, "UPDATE Statuses SET name = ?, percentage = ? WHERE status_id = ?;");
    bindParam($stmt, "sii", $name, $percent, $status_id);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['statuses']);
        return true;
    }
    return false;
}

function deleteStatus($db, $status_id) {
    $stmt = prepareQuery($db, "DELETE FROM Statuses WHERE status_id = ?;");
    bindParam($stmt, "i", $status_id);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['statuses']);
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
        if (updateStatus($mysqli, $_POST['status_id'], $_POST['status_name'], $_POST['status_percent'])) {
            $_SESSION['alert'] .= "alert('$_POST[status_name] was updated.');";
        } else {
            $_SESSION['alert'] .= "alert('Update of $_POST[status_name] failed.');";
        }
    } else if ($_REQUEST['action'] == 'new') {
        if (addStatus($mysqli, $_POST['status_name'], $_POST['status_percent'])) {
            $_SESSION['alert'] .= "alert('$_POST[status_name] was added.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to add $_POST[status_name]. Make sure it does not already exist.');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deleteStatus($mysqli, $_POST['status_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[status_name] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[status_name]. Make sure it is not used by any issues.');";
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
/* get status list */
$statuses = getStatuses($mysqli);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Admin - Statuses</title>
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
    <li class="current">Statuses</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Issue Statuses</h3></li>
    </ul>
    <div class="clearfix">
        <div class="medium-1 column"><strong>ID</strong></div>
        <div class="medium-4 column"><strong>Name</strong></div>
        <div class="medium-4 column"><strong>Percent Complete</strong></div>
        <div class="medium-3 column">&nbsp;</div>
    </div>
<?php if (count($statuses) > 0): ?>
<?php foreach ($statuses as $status): ?>
    <div class="clearfix">
        <form method="POST" action="statuses.php" data-abide>
            <div class="medium-1 column"><?php echo $status['status_id']; ?><input type="hidden" name="status_id" value="<?php echo $status['status_id']; ?>"></div>
            <div class="medium-4 column">
                <input type="text" name="status_name" value="<?php echo $status['name']; ?>" required>
                <small class="error">A status name is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="status_percent" value="<?php echo $status['percentage']; ?>" pattern="[\d]{0,3}" required>
                <small class="error">A completion percentage is required.</small>
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
        <div class="small-12 column">There are no statuses registered.</div>
    </div>
<?php endif ?>
    <div class="clearfix">
        <hr>
        <form method="POST" action="statuses.php" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-4 column">
                <input type="text" name="status_name" placeholder="Status name" required>
                <small class="error">A status name is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="status_percent" placeholder="Percent complete (integer)" pattern="[\d]{0,3}" required>
                <small class="error">A completion percentage is required.</small>
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