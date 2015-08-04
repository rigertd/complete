<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* get UI languages */
$languages = getLanguages($mysqli);

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
    echo "lang request handled";
}
/* check if issue ID was specified */
if (isset($_REQUEST['id'])) {
    $issue = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir, $_REQUEST['id']);
} /* no issue ID specified--redirect to list of all issues */
else {
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/issues.php");
    die();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Issue # <?php echo htmlspecialchars($_REQUEST['id']); ?></title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
</head>
<body>
<?php include('nav.php'); ?>
<?php if (count($issue) == 0): ?>
    <ul class="breadcrumbs">
        <li><a href="index.php">Dashboard</a></li>
        <li class="current">Invalid Issue</li>
    </ul>
    <div class="medium-12 columns">
        <h3>Issue #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3>
        <div class="alert-box alert">Issue not found or access denied.</div>
    </div>
<?php else: ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li><a href="project.php?id=<?php echo $issue[0]['proj_id']; ?>"><?php echo htmlspecialchars($issue[0]['proj_name']); ?></a></li>
    <li class="current">Issue #<?php echo $issue[0]['issue_id']; ?></li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Issue #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3></li>
        <li><a href="issue.php?id=<?php echo htmlspecialchars($_REQUEST['id']); ?>&action=update" class="button radius tiny">Update</a></li>
    </ul>
    <div class="medium-12 panel clearfix">
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Subject:</strong></div>
            <div class="medium-10 large-10 column"><?php echo htmlspecialchars($issue[0]['subject']); ?> </div>
        </div>
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Assignee:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['assignee_name']); ?>&nbsp;</div>
            <div class="medium-2 large-2 column"><strong>Created:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['created_on']); ?>&nbsp;</div>
        </div>
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Priority:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['priority']); ?>&nbsp;</div>
            <div class="medium-2 large-2 column"><strong>Updated:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['last_update']); ?>&nbsp;</div>
        </div>
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Status:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['status']); ?>&nbsp;</div>
            <div class="medium-2 large-2 column"><strong>Due&nbsp;Date:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['due_date']); ?>&nbsp;</div>
        </div>
        <div class="clearfix">
            <div class="medium-2 large-2 column"><strong>Project:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['proj_name']); ?>&nbsp;</div>
            <div class="medium-2 large-2 column"><strong>Completed:</strong></div>
            <div class="medium-4 large-4 column"><?php echo htmlspecialchars($issue[0]['completed_on']); ?>&nbsp;</div>
        </div>
        <div class="clearfix">
            <div class="medium-12 large-2 column"><strong>Description:</strong> </div>
            <div class="medium-12 large-10 column"><?php echo nl2br(htmlspecialchars($issue[0]['description'])); ?>&nbsp;</div>
        </div>
        <div><hr></div>
        <ul class="inline-list" style="margin-bottom: 0;">
            <li style="margin-left: 0;"><h4>Related Issues <a href="#" title="Add Related Issue"><i class="fi-plus"></i></a></h4></li>
        </ul>
        <table>
            <thead>
            <tr>
                <th>Issue #</th>
                <th>Subject</th>
                <th>Relation</th>
                <th>Status</th>
                <th></th>
            </tr>
            </thead>
            <tbody>
            <tr>
                <td><a href="issue.php?id=1">1</a></td>
                <td><a href="issue.php?id=1">Sample Issue</a></td>
                <td>Blocks</td>
                <td>New</td>
                <td><a href="issue.php?id=1&remove=2" title="Remove Related Issue"><i class="fi-trash"></i></a></td>
            </tr>
            </tbody>
        </table>
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
