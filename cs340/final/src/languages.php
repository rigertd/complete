<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* language administration functions */
function addLanguage($db, $name, $langCode, $localeCode) {
    $stmt = prepareQuery($db, "INSERT INTO Languages (name, lang_code, locale_code) VALUES (?, ?, ?);");
    bindParam($stmt, "sss", $name, $langCode, $localeCode);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['languages']);
        return true;
    }
    return false;
}

function updateLanguage($db, $langId, $name, $langCode, $localeCode) {
    $stmt = prepareQuery($db, "UPDATE Languages SET name = ?, lang_code = ?, locale_code = ? WHERE lang_id = ?;");
    bindParam($stmt, "sssi", $name, $langCode, $localeCode, $langId);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['languages']);
        return true;
    }
    return false;
}

function deleteLanguage($db, $langId) {
    $stmt = prepareQuery($db, "DELETE FROM Languages WHERE lang_id = ?;");
    bindParam($stmt, "i", $langId);
    executeStatement($stmt);
    if ($stmt->affected_rows > 0) {
        unset($_SESSION['languages']);
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
        if (updateLanguage($mysqli, $_POST['lang_id'], $_POST['lang_name'], $_POST['lang_code'], $_POST['locale_code'])) {
            $_SESSION['alert'] .= "alert('$_POST[lang_name] was updated.');";
        } else {
            $_SESSION['alert'] .= "alert('Update of $_POST[lang_name] failed.');";
        }
    } else if ($_REQUEST['action'] == 'new') {
        if (addLanguage($mysqli, $_POST['lang_name'], $_POST['lang_code'], $_POST['locale_code'])) {
            $_SESSION['alert'] .= "alert('$_POST[lang_name] was added.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to add $_POST[lang_name]. Make sure it does not already exist.');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deleteLanguage($mysqli, $_POST['lang_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[lang_name] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[lang_name]. Make sure it is not used in any issues or comments');";
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

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Admin - Languages</title>
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
    <li class="current">Languages</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Supported Languages</h3></li>
    </ul>
    <div class="clearfix">
        <div class="medium-1 column"><strong>ID</strong></div>
        <div class="medium-3 column"><strong>Name</strong></div>
        <div class="medium-3 column"><strong>Language Code</strong></div>
        <div class="medium-3 column"><strong>Locale Code</strong></div>
        <div class="medium-2 column">&nbsp;</div>
    </div>
<?php if (count($languages) > 0): ?>
<?php foreach ($languages as $lang): ?>
    <div class="clearfix">
        <form method="POST" action="languages.php" data-abide>
            <div class="medium-1 column"><?php echo $lang['lang_id']; ?><input type="hidden" name="lang_id" value="<?php echo $lang['lang_id']; ?>"></div>
            <div class="medium-3 column">
                <input type="text" name="lang_name" value="<?php echo $lang['name']; ?>" required>
                <small class="error">A language name is required.</small>
            </div>
            <div class="medium-3 column">
                <input type="text" name="lang_code" value="<?php echo $lang['lang_code']; ?>" required pattern="[a-zA-Z]{2}">
                <small class="error">An ISO language code is required.</small>
            </div>
            <div class="medium-3 column"><input type="text" name="locale_code" value="<?php echo $lang['locale_code']; ?>" pattern="[a-zA-Z]{2}"></div>
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
        <div class="small-12 column">There are no languages registered.</div>
    </div>
<?php endif ?>
    <div class="clearfix">
        <hr>
        <form method="POST" action="languages.php" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-3 column">
                <input type="text" name="lang_name" placeholder="Language name" required>
                <small class="error">A language name is required.</small>
            </div>
            <div class="medium-3 column">
                <input type="text" name="lang_code" placeholder="2-digit ISO code" required pattern="[a-zA-Z]{2}">
                <small class="error">An ISO language code is required.</small>
            </div>
            <div class="medium-3 column"><input type="text" name="locale_code" placeholder="2-digit locale code" pattern="[a-zA-Z]{2}"></div>
            <div class="medium-2 column">
                <ul class="button-group radius">
                    <li><button type="submit" title="Add" class="button tiny" name="action" value="new"><i class="fi-plus" style="font-size: 150%"></i></button></li>
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