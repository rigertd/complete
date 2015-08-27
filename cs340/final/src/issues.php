<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/**
 * Adds a new issue to a project.
 * @param $db
 * @param $user_id
 * @param $priority_id
 * @param $status_id
 * @param $lang_id
 * @param $assignee
 * @param $due_date
 * @param $proj_id
 * @return int
 */
function addIssue($db, $user_id, $priority_id, $status_id, $lang_id, $assignee, $due_date, $proj_id) {
    global $CREATE;
    global $PROJECT;
    if (trim($assignee) == '') {
        $assignee = null;
    }
    if (trim($due_date) == '') {
        $due_date = null;
    }
    $query = "INSERT INTO Issues (priority_id, status_id, lang_id, assignee, created_by, due_date, completed_on, proj_id) ".
             "SELECT DISTINCT ?, ?, $lang_id, ?, $user_id, ?, ".
             "CASE WHEN ? IN (SELECT status_id FROM Statuses WHERE complete = 1) THEN NOW() ELSE NULL END, ? FROM Users_Projects up ".
             "WHERE (up.user_id = $user_id AND (role = $CREATE OR role = $PROJECT)) ".
             "OR EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1);";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iiisii", $priority_id, $status_id, $assignee, $due_date, $status_id, $proj_id);
    executeStatement($stmt);
    return $stmt->insert_id;
}

/**
 * Deletes an issue.
 * This is only ever called if something goes wrong with adding issue text.
 * @param $db
 * @param $issue_id
 */
function deleteIssue($db, $issue_id) {
    $stmt = prepareQuery($db, "DELETE FROM Issues WHERE issue_id = ?;");
    bindParam($stmt, "i", $issue_id);
    executeStatement($stmt);
}

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
} else if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'new') {
    /* create a new issue and direct to the issue details page if successful */
    $newId = addIssue($mysqli, $user_id, $_REQUEST['priority'], $_REQUEST['status'], $ui_lang_id, $_REQUEST['assignee'], $_REQUEST['duedate'].' '.$_REQUEST['duetime'], $_REQUEST['project']);
    $url = $_SERVER['HTTP_HOST'].rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    if (!is_null($newId) && $newId > 0) {
        if (addIssueText($mysqli, $newId, $ui_lang_id, $_REQUEST['subject'], $_REQUEST['description'])) {
            $url .= "/issue.php?id=$newId";
        } else {
            /* delete issue if failed to add issue text */
            deleteIssue($mysqli, $newId);
            $url .= "/issues.php";
        }
    } else {
        $url .= "/issues.php";
    }
    header("Location: https://$url");
    die();
}

/* get UI languages */
$languages = getLanguages($mysqli);

/* check if project ID was specified */
if (isset($_REQUEST['id'])) {
    /* get project issues */
    $issues = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir, NULL, NULL, $_REQUEST['id']);
    $project = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir, $_REQUEST['id']);
    $user_roles = getProjectRoles($mysqli, $user_id, $_REQUEST['id']);
    $is_project = isset($project[0]);
    $proj_users = getProjectUsers($mysqli, $user_id, $_REQUEST['id']);
} else {
    /* get all issues */
    $issues = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir);
    $is_project = false;
    $proj_users = getAllUsers($mysqli);
}
$projects = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir);
$priorities = getPriorities($mysqli);
$statuses = getStatuses($mysqli);

/* permission checking */
$is_admin = isAdmin($mysqli, $user_id);

$sort_arrow = $issue_sort_dir == 'ASC' ? '<i class="fi-arrow-up"></i>' : '<i class="fi-arrow-down"></i>';
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Issue List</title>
    <link href="../css/foundation.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../css/foundation-icons.css" />
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/vendor/modernizr.js"></script>
    <script>
        function showNew() {
            document.getElementById('new_issue').style.display = '';
            document.getElementById('new_button').style.display = 'none';
        }
        function hideNew() {
            document.getElementById('new_issue').style.display = 'none';
            document.getElementById('new_button').style.display = '';
        }
    </script>
</head>
<body>
<?php include('nav.php'); ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
<?php if ($is_project): ?>
    <li><a href="projects.php">Project List</a></li>
    <li><a href="project.php?id=<?php echo $project[0]['proj_id']; ?>"><?php echo htmlspecialchars($project[0]['name']); ?></a></li>
<?php endif ?>
    <li class="current">Issue List</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
<?php if ($is_project): ?>
        <li><h3><?php echo htmlspecialchars($project[0]['name']); ?> Issues</h3></li>
<?php else: ?>
        <li><h3>All Issues</h3></li>
<?php endif ?>
<?php if ($is_admin || isset($user_roles[$PROJECT]) || isset($user_roles[$CREATE])): ?>
        <li id="new_button">
            <ul class="inline-list">
                <li><button class="button tiny radius" onclick="showNew()">New Issue</button></li>
            </ul>
        </li>
<?php endif ?>
    </ul>
    <div class="medium-12 panel" id="new_issue" style="display:none;">
        <form action="issues.php" method="POST" data-abide>
            <div class="clearfix">
                <div class="medium-2 column"><label for="subject">Subject</label></div>
                <div class="medium-10 column">
                    <input type="text" id="subject" name="subject" placeholder="Enter a subject. (Required)" required>
                    <small class="error">A subject is required.</small>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="project">Project</label></div>
                <div class="medium-4 column end">
                    <select name="project" required>
                        <option value="" disabled>Select a Project (Required)</option>
<?php foreach ($projects as $proj): ?>
                        <option value="<?php echo $proj['proj_id']; ?>"<?php if ($is_project && $proj['proj_id'] == $_REQUEST['id']) {echo " selected";} ?>><?php echo $proj['name'] ?></option>
<?php endforeach ?>
                    </select>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="assignee">Assignee</label></div>
                <div class="medium-4 column end">
                    <select name="assignee">
                        <option value="">None</option>
<?php foreach ($proj_users as $user): ?>
                        <option value="<?php echo $user['user_id']; ?>"><?php echo $user['first_name'].' '.$user['last_name'] ?></option>
<?php endforeach ?>
                    </select>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="priority">Priority</label></div>
                <div class="medium-4 column end">
                    <select name="priority" required>
<?php foreach ($priorities as $priority): ?>
                        <option value="<?php echo $priority['priority_id']; ?>"><?php echo $priority['name'] ?></option>
<?php endforeach ?>
                    </select>
                    <small class="error">A priority is required.</small>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="status">Status</label></div>
                <div class="medium-4 column end">
                    <select name="status" required>
<?php foreach ($statuses as $status): ?>
                        <option value="<?php echo $status['status_id']; ?>"><?php echo $status['name'] ?></option>
<?php endforeach ?>
                    </select>
                    <small class="error">A status is required.</small>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="duedate">Due&nbsp;Date</label></div>
                <div class="medium-4 column end">
                    <div class="row collapse">
                        <div class="medium-6 column"><input id="duedate" name="duedate" type="date" placeholder="YYYY-MM-DD"></div>
                        <div class="medium-6 column"><input id="duetime" name="duetime" type="time" placeholder="HH:MM:SS"></div>
                    </div>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-12 large-2 column"><label for="description">Description</label> </div>
                <div class="medium-12 large-10 column">
                    <textarea id="description" name="description" rows="5" maxlength="2000" placeholder="Enter a description. (Optional)"></textarea>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" class="button tiny" name="action" value="new">Add</button></li>
                        <li><button class="button tiny" onclick="hideNew();return false;">Cancel</button></li>
                    </ul>
                </div>
            </div>
        </form>
    </div>
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
            <td><a href="issue.php?id=<?php echo $issue['issue_id']; ?>"><?php if ($issue['complete']) {echo "<s>";} echo $issue['issue_id']; if ($issue['complete']) {echo "</s>";} ?></a></td>
            <td><a href="issue.php?id=<?php echo $issue['issue_id']; ?>"><?php if ($issue['complete']) {echo "<s>";} echo htmlspecialchars($issue['subject']) . (!$issue['translated'] ? ' (Not Translated)' : ''); if ($issue['complete']) {echo "<s>";} ?></a></td>
            <td><?php echo htmlspecialchars($issue['status']); ?></td>
            <td><?php echo htmlspecialchars($issue['priority']); ?></td>
            <td><?php echo htmlspecialchars($issue['assignee_name']); ?></td>
            <td><a href="project.php?id=<?php echo $issue['proj_id']; ?>"><?php echo htmlspecialchars($issue['proj_name']); ?></a></td>
            <td><?php echo htmlspecialchars($issue['due_date']); ?></td>
        </tr>
<?php endforeach ?>
<?php else: ?>
        <tr>
            <td colspan="7">No issues found.</td>
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