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
    <ul class="tabs" data-tab>
        <li class="tab-title active"><a href="#overview">Overview</a></li>
        <li class="tab-title"><a href="#issues">Issues</a></li>
        <li class="tab-title"><a href="#users">Users</a></li>
    </ul>
    <div class="tabs-content">
        <div class="content active" id="overview">
            <div class="medium-12 clearfix">
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
        <div class="content" id="users">
            <ul class="inline-list" style="margin-bottom: 0;">
                <li style="margin-left: 0;"><h4>Project Users <a href="#" title="Add User"><i class="fi-plus"></i></a></h4></li>
            </ul>
            <table>
                <thead>
                <tr>
                    <th>Username</th>
                    <th>Role</th>
                    <th></th>
                </tr>
                </thead>
                <tbody>
                <tr>
                    <td>translator</td>
                    <td>Translator</td>
                    <td><a href="issue.php?id=1&remove=2" title="Remove Related Issue"><i class="fi-trash"></i></a></td>
                </tr>
                </tbody>
            </table>
        </div>
        <div class="content" id="issues">
            <ul class="inline-list" style="margin-bottom: 0;">
                <li style="margin-left: 0;"><h4>Project Issues</h4></li>
                <li><a href="project.php" class="button radius tiny">New Issue</a></li>
            </ul>
            <table>
                <thead>
                <tr>
                    <th><a href="?sort=1" title="Sort by Issue #">Issue&nbsp;#&nbsp;<?php echo ($issue_sort_col == 1 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=2" title="Sort by Subject">Subject&nbsp;<?php echo ($issue_sort_col == 2 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=3" title="Sort by Status">Status&nbsp;<?php echo ($issue_sort_col == 3 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=4" title="Sort by Priority">Priority&nbsp;<?php echo ($issue_sort_col == 4 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=5" title="Sort by Assignee">Assignee&nbsp;<?php echo ($issue_sort_col == 5 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=6" title="Sort by Project">Project&nbsp;<?php echo ($issue_sort_col == 6 ? $sort_arrow : ''); ?></a></th>
                    <th><a href="?sort=7" title="Sort by Due Date">Due&nbsp;Date&nbsp;<?php echo ($issue_sort_col == 7 ? $sort_arrow : ''); ?></a></th>
                </tr>
                </thead>
                <tbody>
<?php if (count($issues) > 0): ?>
<?php foreach ($issues as $issue): ?>
                <tr>
                    <td><a href="issue.php?id=<?php echo $issue['issue_id']; ?>"><?php echo $issue['issue_id']; ?></a></td>
                    <td><a href="issue.php?id=<?php echo $issue['issue_id']; ?>"><?php echo htmlspecialchars($issue['subject']) . (!$issue['translated'] ? ' (Not Translated)' : ''); ?></a></td>
                    <td><?php echo htmlspecialchars($issue['status']); ?></td>
                    <td><?php echo htmlspecialchars($issue['priority']); ?></td>
                    <td><?php echo htmlspecialchars($issue['assignee_name']); ?></td>
                    <td><a href="project.php?id=<?php echo $issue['proj_id']; ?>"><?php echo htmlspecialchars($issue['proj_name']); ?></a></td>
                    <td><?php echo htmlspecialchars($issue['due_date']); ?></td>
                </tr>
<?php endforeach ?>
<?php else: ?>
                <tr>
                    <td colspan="7">This project does not have any issues.</td>
                </tr>
<?php endif ?>
                </tbody>
            </table>
        </div>
    </div>
</div>
<?php endif ?>
<div class="medium-12 columns text-center">&copy; David Rigert</div>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation/foundation.js"></script>
<script src="../js/foundation/foundation.topbar.js"></script>
<script src="../js/foundation/foundation.tab.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>
