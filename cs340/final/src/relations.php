<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* relationship administration functions */
function addRelationship($db, $forward_desc, $reverse_desc) {
    $stmt = prepareQuery($db, "INSERT INTO Relationships (forward_desc, reverse_desc) VALUES (?, ?);");
    bindParam($stmt, "ss", $forward_desc, $reverse_desc);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['relationships']);
        return true;
    }
    return false;
}

function updateRelationship($db, $rel_id, $forward_desc, $reverse_desc) {
    $stmt = prepareQuery($db, "UPDATE Relationships SET forward_desc = ?, reverse_desc = ? WHERE rel_id = ?;");
    bindParam($stmt, "ssi", $forward_desc, $reverse_desc, $rel_id);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['relationships']);
        return true;
    }
    return false;
}

function deleteRelationship($db, $rel_id) {
    $stmt = prepareQuery($db, "DELETE FROM Relationships WHERE rel_id = ?;");
    bindParam($stmt, "i", $rel_id);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['relationships']);
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
        if (updateRelationship($mysqli, $_POST['rel_id'], $_POST['forward_desc'], $_POST['reverse_desc'])) {
            $_SESSION['alert'] .= "alert('$_POST[forward_desc] was updated.');";
        } else {
            $_SESSION['alert'] .= "alert('Update of $_POST[forward_desc] failed.');";
        }
    } else if ($_REQUEST['action'] == 'new') {
        if (addRelationship($mysqli, $_POST['forward_desc'], $_POST['reverse_desc'])) {
            $_SESSION['alert'] .= "alert('$_POST[forward_desc] was added.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to add $_POST[forward_desc]. Make sure it does not already exist');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deleteRelationship($mysqli, $_POST['rel_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[forward_desc] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[forward_desc]. Make sure it is not used for any related issues.');";
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
$relationships = getRelationships($mysqli);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Admin - Relationships</title>
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
    <li class="current">Relationships</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Issue Relationships</h3></li>
    </ul>
    <div class="clearfix">
        <div class="medium-1 column"><strong>ID</strong></div>
        <div class="medium-4 column"><strong>Forward Description</strong></div>
        <div class="medium-4 column"><strong>Reverse Description</strong></div>
        <div class="medium-3 column">&nbsp;</div>
    </div>
<?php if (count($relationships) > 0): ?>
<?php foreach ($relationships as $rel): ?>
    <div class="clearfix">
        <form method="POST" action="relations.php" data-abide>
            <div class="medium-1 column"><?php echo $rel['rel_id']; ?><input type="hidden" name="rel_id" value="<?php echo $rel['rel_id']; ?>"></div>
            <div class="medium-4 column">
                <input type="text" name="forward_desc" value="<?php echo $rel['forward_desc']; ?>" required>
                <small class="error">A description of the forward relationship is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="reverse_desc" value="<?php echo $rel['reverse_desc']; ?>" required>
                <small class="error">A description of the reverse relationship is required.</small>
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
        <div class="small-12 column">There are no relationships registered.</div>
    </div>
<?php endif ?>
    <div class="clearfix">
        <hr>
        <form method="POST" action="relations.php" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-4 column">
                <input type="text" name="forward_desc" placeholder="e.g. Blocks" required>
                <small class="error">A description of the forward relationship is required.</small>
            </div>
            <div class="medium-4 column">
                <input type="text" name="reverse_desc" placeholder="e.g. Blocked by" required>
                <small class="error">A description of the reverse relationship is required.</small>
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