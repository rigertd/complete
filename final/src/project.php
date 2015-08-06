<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* get UI languages */
$languages = getLanguages($mysqli);

/* toggle input form visibility */
$showForm = false;

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
    echo "lang request handled";
}
/* check if project ID was specified */
if (isset($_REQUEST['id'])) {
    $project = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir, $_REQUEST['id']);
} /* check if creating a new project */
else if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'new') {

} /* no project ID or new specified--redirect to list of all projects */
else {
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/projects.php");
    die();
}

$is_admin = isAdmin($mysqli, $user_id);
$user_roles = getProjectRoles($mysqli, $user_id, $_REQUEST['id']);

$users = getAllUsers($mysqli);
$proj_users = getProjectUsersRoles($mysqli, $user_id, $_REQUEST['id']);
$issues = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir, NULL, NULL, $project[0]['proj_id']);
$sort_arrow = $issue_sort_dir == 'ASC' ? '<i class="fi-arrow-up"></i>' : '<i class="fi-arrow-down"></i>';

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Project # <?php echo htmlspecialchars($_REQUEST['id']); ?></title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
</head>
<body>
<?php include('nav.php'); ?>
<?php if (count($project) == 0): ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="current">Invalid Project</li>
</ul>
<div class="medium-12 columns">
    <h3>Project #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3>
    <div class="alert-box alert">Project not found or access denied.</div>
</div>
<?php else: ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="current"><?php echo htmlspecialchars($project[0]['name']); ?></li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom:0;">
        <li><h3>Project Overview</h3></li>
        <li>
            <ul class="button-group radius">
                <li><a href="issues.php?id=<?php echo $project[0]['proj_id']; ?>" class="button tiny">Issues</a></li>
                <li><a href="proj_users.php?id=<?php echo $project[0]['proj_id']; ?>" class="button tiny">Users</a></li>
            </ul>
        </li>
    </ul>
    <div class="medium-12 panel">
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Name:</strong></div>
            <div class="medium-10 large-10 column"><?php echo htmlspecialchars($project[0]['name']); ?> </div>
        </div>
        <div class="clearfix">
            <div class="medium-12 large-2 column"><strong>Description:</strong> </div>
            <div class="medium-12 large-10 column"><?php echo nl2br(htmlspecialchars($project[0]['description'])); ?>&nbsp;</div>
        </div>
    </div>
</div>
<?php endif ?>
<div class="medium-12 columns text-center">&copy; David Rigert</div>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation/foundation.js"></script>
<script src="../js/foundation/foundation.topbar.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>
