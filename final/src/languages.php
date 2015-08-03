<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

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
            $_SESSION['alert'] .= "alert('Failed to add $_POST[lang_name].');";
        }
    } else if ($_REQUEST['action'] == 'delete') {
        if (deleteLanguage($mysqli, $_POST['lang_id'])) {
            $_SESSION['alert'] .= "alert('$_POST[lang_name] was deleted.');";
        } else {
            $_SESSION['alert'] .= "alert('Failed to delete $_POST[lang_name].');";
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
/* get language list */
$projects = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir);


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
                <input type="text" name="lang_code" value="<?php echo $lang['lang_code']; ?>" required>
                <small class="error">An ISO language code is required.</small>
            </div>
            <div class="medium-3 column"><input type="text" name="locale_code" value="<?php echo $lang['locale_code']; ?>"></div>
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
        <form method="POST" action="languages.php?action=new" data-abide>
            <div class="medium-1 column">New</div>
            <div class="medium-3 column">
                <input type="text" name="lang_name" placeholder="Language name" required>
                <small class="error">A language name is required.</small>
            </div>
            <div class="medium-3 column">
                <input type="text" name="lang_code" placeholder="2-digit ISO code" required>
                <small class="error">An ISO language code is required.</small>
            </div>
            <div class="medium-3 column"><input type="text" name="locale_code" placeholder="2-digit locale code"></div>
            <div class="medium-2 column">
                <ul class="button-group radius">
                    <li><button type="submit" title="Save" class="button tiny"><i class="fi-plus" style="font-size: 150%"></i></button></li>
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
<script src="../js/foundation/foundation.reveal.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>