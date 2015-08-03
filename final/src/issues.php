<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
} /* sort issue list */
else if (isset($_REQUEST['sort'])) {
    /* reverse if col is the same */
    if ($issue_sort_col == $_REQUEST['sort']) {
        $issue_sort_dir = $issue_sort_dir == 'ASC' ? 'DESC' : 'ASC';
        $_SESSION['issue_sort_dir'] = $issue_sort_dir;
    } /* input validation */
    else if ($_REQUEST['sort'] > 0 && $_REQUEST['sort'] < 8) {
        $issue_sort_col = $_REQUEST['sort'];
        $_SESSION['issue_sort_col'] = $issue_sort_col;
    }

    // redirect to remove URL parameters
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    $url = strtok($url, '?');
    header("Location: https://$url");
    die();
}

/* get UI languages */
$languages = getLanguages($mysqli);
/* get issue list */
$issues = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir);


$sort_arrow = $issue_sort_dir == 'ASC' ? '<i class="fi-arrow-up"></i>' : '<i class="fi-arrow-down"></i>';

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - User Dashboard</title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
</head>
<body>
<?php include('nav.php'); ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="current">Issue List</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>All Issues</h3></li>
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
                <td colspan="7">You do not have any issues assigned to you.</td>
            </tr>
        <?php endif ?>
        </tbody>
    </table>
</div>
<div class="medium-12 columns text-center">&copy; David Rigert</div>
<script src="../js/vendor/jquery.js"></script>
<script src="../js/foundation/foundation.js"></script>
<script src="../js/foundation/foundation.topbar.js"></script>
<script>
    $(document).foundation();
</script>
</body>
</html>