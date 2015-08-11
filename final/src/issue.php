<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/**
 * Gets all issues related to the specified issue.
 * Only gets issues that the user has access to.
 * @param mysqli $db    The mysqli database instance.
 * @param int $user_id  The user_id of the requester.
 * @param int $issue_id The issue_id to get related issues of.
 * @return array
 */
function getRelatedIssues($db, $user_id, $issue_id) {
    $query = "SELECT ir.issue_id1 AS issue_id, ir.issue_id2 AS rel_issue_id, ir.rel_id, r.forward_desc AS descrip, 'f' AS dir ".
        "FROM Issues_Relationships ir ".
        "INNER JOIN Relationships r ON ir.rel_id = r.rel_id ".
        "INNER JOIN Issues i ON ir.issue_id1 = i.issue_id ".
        "LEFT JOIN Users_Projects up ON i.proj_id = up.proj_id ".
        "WHERE ir.issue_id1 = ? AND (up.user_id = ? OR EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1))".
        "UNION ".
        "SELECT ir.issue_id2 AS issue_id, ir.issue_id1 AS rel_issue_id, ir.rel_id, r.reverse_desc AS descrip, 'r' AS dir ".
        "FROM Issues_Relationships ir ".
        "INNER JOIN Relationships r ON ir.rel_id = r.rel_id ".
        "INNER JOIN Issues i ON ir.issue_id2 = i.issue_id ".
        "LEFT JOIN Users_Projects up ON i.proj_id = up.proj_id ".
        "WHERE ir.issue_id2 = ? AND (up.user_id = ? OR EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1))";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iiii", $issue_id, $user_id, $issue_id, $user_id);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $rel_issues = array();
    while ($row = $results->fetch_assoc()) {
        $rel_issues[] = $row;
    }
    return $rel_issues;
}

/**
 * Links an issue to the specified issue with the specified relation.
 * @param $db
 * @param $issue_id
 * @param $rel_issue_id
 * @param $rel_id
 * @return bool
 */
function addRelatedIssue($db, $issue_id, $rel_issue_id, $rel_id) {
    $query = "INSERT INTO Issues_Relationships (issue_id1, issue_id2, rel_id) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE rel_id = ?;";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iiii", $issue_id, $rel_issue_id, $rel_id, $rel_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

/**
 * Removes a related issue. Removes both directions just in case.
 * @param $db
 * @param $issue_id
 * @param $rel_issue_id
 * @return bool
 */
function deleteRelatedIssue($db, $issue_id, $rel_issue_id) {
    $query = "DELETE FROM Issues_Relationships WHERE issue_id1 = ? AND issue_id2 = ?;";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ii", $issue_id, $rel_issue_id);
    executeStatement($stmt);
    if ($stmt->affected_rows == 0) {
        bindParam($stmt, "ii", $rel_issue_id, $issue_id);
        executeStatement($stmt);
    }
    return $stmt->affected_rows > 0;
}

function hasId($arr, $id) {
    foreach ($arr as $row) {
        if ($row['issue_id'] == $id || $row['rel_issue_id'] == $id) {
            return true;
        }
    }
    return false;
}

/**
 * Gets all of the updates for a particular issue.
 * Gets the localized text if available, or the source text if not.
 * @param mysqli $db    The mysqli database instance.
 * @param int $issue_id The issue_id of the issue.
 * @param int $lang_id  The lang_id of the language to get.
 * @return array
 */
function getIssueUpdates($db, $issue_id, $lang_id) {
    $query = "SELECT up.update_id, u.user_id, ".
        "       CONCAT(u.first_name, ' ', COALESCE(u.last_name, '')) AS full_name, ".
        "       up.created_on, COALESCE(t_ul.description, s_ul.description, '') AS comment, ".
        "       CASE WHEN t_ul.description IS NULL AND s_ul.description IS NOT NULL THEN 0 ELSE 1 END AS translated ".
        "FROM Updates up INNER JOIN Users u ON up.user_id = u.user_id ".
        "LEFT JOIN (SELECT update_id, description FROM Updates_Languages WHERE lang_id = $lang_id) AS t_ul ON up.update_id = t_ul.update_id ".
        "LEFT JOIN Updates_Languages s_ul ON up.update_id = s_ul.update_id AND up.lang_id = s_ul.lang_id ".
        "WHERE up.issue_id = ? ".
        "ORDER BY up.created_on DESC";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "i", $issue_id);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $updates = array();
    while ($row = $results->fetch_assoc()) {
        $updates[] = $row;
    }
    return $updates;
}

/**
 * Updates an issue to the specified information.
 * Changes the source language if the update is in a different language.
 * Sets the completion date if a complete status is set.
 * @param $db
 * @param $user_id
 * @param $issue_id
 * @param $lang_id
 * @param $priority_id
 * @param $status_id
 * @param $assignee_id
 * @param $due
 * @param $proj_id
 * @return bool
 */
function updateIssue($db, $user_id, $issue_id, $lang_id, $priority_id, $status_id, $assignee_id, $due, $proj_id) {
    global $UPDATE;
    global $PROJECT;
    if (trim($assignee_id) == '') {
        $assignee_id = null;
    }
    if (trim($due) == '') {
        $due = null;
    }
    $query = "UPDATE Issues i ".
        "SET i.priority_id = ?, i.status_id = ?, i.assignee = ?, i.due_date = ?, i.lang_id = ?, i.proj_id = ?, ".
        "    i.completed_on = CASE WHEN ? IN (SELECT status_id FROM Statuses WHERE complete = 1) THEN NOW() ELSE NULL END ".
        "WHERE i.issue_id = ? AND ".
        "(EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1) ".
        " OR $user_id IN (SELECT user_id FROM Users_Projects WHERE proj_id = ? AND (role = $PROJECT OR role = $UPDATE)));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iiisiiiii", $priority_id, $status_id, $assignee_id, $due, $lang_id, $proj_id, $status_id, $issue_id,$proj_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

function updateIssueText($db, $user_id, $lang_id, $issue_id, $subject, $description) {
    global $UPDATE;
    global $PROJECT;
    global $TRANSLATE;
    $query = "UPDATE Issues_Languages il ".
        "INNER JOIN Issues i ON il.issue_id = i.issue_id ".
        "LEFT JOIN Users_Projects up ON i.proj_id = up.proj_id AND (up.role = $PROJECT OR up.role = $UPDATE OR up.role = $TRANSLATE) ".
        "SET il.subject = ?, il.description = ? ".
        "WHERE il.lang_id = $lang_id AND il.issue_id = ? AND ".
        "(up.user_id = $user_id OR EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ssi", $subject, $description, $issue_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

/**
 * Adds a new update for an issue. Only creates a comment entry if one was entered.
 * Deletes the update if a comment was entered but could not be added to DB.
 * @param $db
 * @param $user_id
 * @param $lang_id
 * @param $issue_id
 * @param $comment
 * @return bool
 */
function addIssueUpdate($db, $user_id, $lang_id, $issue_id, $comment) {
    global $UPDATE;
    global $PROJECT;
    $query = "INSERT INTO Updates (issue_id, user_id, lang_id) ".
        "SELECT DISTINCT i.issue_id, $user_id, ? FROM Issues i ".
        "LEFT JOIN Users_Projects up ON i.proj_id = up.proj_id AND (up.role = $UPDATE OR up.role = $PROJECT) ".
        "WHERE i.issue_id = ? AND ".
        "(up.user_id = $user_id OR EXISTS (SELECT * FROM Users WHERE user_id = $user_id AND admin = 1));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ii", $lang_id, $issue_id);
    executeStatement($stmt);
    $update_id = $stmt->insert_id;
    /* if addition was successful and a comment was entered, add text */
    if ($update_id != null && $stmt->affected_rows > 0 && strlen(trim($comment)) > 0) {
        /* delete update if text insertion failed */
        if (!addUpdateComment($db, $update_id, $lang_id, $comment)) {
            $db->query("DELETE FROM Updates WHERE update_id = $update_id");
            return false;
        }
    }
    return $stmt->affected_rows > 0;
}

/**
 * Adds a comment for the specified update in the specified language.
 * @param $db
 * @param $update_id
 * @param $lang_id
 * @param $comment
 * @return bool
 */
function addUpdateComment($db, $update_id, $lang_id, $comment) {
    $query = "INSERT INTO Updates_Languages (update_id, lang_id, description) VALUES (?, ?, ?);";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iis", $update_id, $lang_id, $comment);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

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
    $issues = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir);
    $rels = getRelationships($mysqli);
    $rel_issues = getRelatedIssues($mysqli, $user_id, $_REQUEST['id']);
    $proj_users = getProjectUsers($mysqli, $user_id, $issue[0]['proj_id']);
    $priorities = getPriorities($mysqli);
    $statuses = getStatuses($mysqli);
    $projects = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir);
    $outdated = getOutdatedIssueTrans($mysqli, $_REQUEST['id'], $ui_lang_id);
    $updates = getIssueUpdates($mysqli, $_REQUEST['id'], $ui_lang_id);
} /* no issue ID specified--redirect to list of all issues */
else {
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/issues.php");
    die();
}

/* permission checking */
$roles = getProjectRoles($mysqli, $user_id, $issue[0]['proj_id']);
$is_admin = isAdmin($mysqli, $user_id);

/* handle form actions */
if (isset($_REQUEST['action'])) {
    /* check for proper permissions (admin, PM, or update) */
    if ($is_admin || isset($roles[$PROJECT]) || isset($roles[$UPDATE]) || isset($roles[$TRANSLATE])) {
        if ($_REQUEST['action'] == 'add_rel') {
            /* add a new relationship */
            $rel_info = explode("_", $_REQUEST['rel_type']);
            $rel_dir = $rel_info[0];
            $rel_id = $rel_info[1];
            /* check if related issue already exists */
            if (!hasId($rel_issues, $_REQUEST['issue_id'])) {
                /* check direction of relationship */
                if ($rel_dir == 'f') {
                    addRelatedIssue($mysqli, $_REQUEST['id'], $_REQUEST['issue_id'], $rel_id);
                } else if ($rel_dir = 'r') {
                    addRelatedIssue($mysqli, $_REQUEST['issue_id'], $_REQUEST['id'], $rel_id);
                }
            }
        } else if ($_REQUEST['action'] == 'del_rel') {
            /* delete a relationship */
            deleteRelatedIssue($mysqli, $_REQUEST['id'], $_REQUEST['rel_id']);
        } else if ($_REQUEST['action'] == 'update') {
            /* update the issue */
            updateIssue($mysqli, $user_id, $_REQUEST['id'], $ui_lang_id, $_REQUEST['priority'], $_REQUEST['status'], $_REQUEST['assignee'], $_REQUEST['duedate'].' '.$_REQUEST['duetime'], $_REQUEST['project']);
            updateIssueText($mysqli, $user_id, $ui_lang_id, $_REQUEST['id'], $_REQUEST['subject'], $_REQUEST['description'], $_REQUEST['project']);
            addIssueUpdate($mysqli, $user_id, $ui_lang_id, $_REQUEST['id'], $_REQUEST['comment'], $_REQUEST['project']);
        } else if ($_REQUEST['action'] == 'translate') {
            /* provide a new or updated translation */
            if ($is_admin || isset($roles[$TRANSLATE])) {
                addIssueText($mysqli, $_REQUEST['id'], $ui_lang_id, $_REQUEST['subject'], $_REQUEST['description']);
            }
        } else if ($_REQUEST['action'] == 'trans_update') {
            /* provide a translation of an update comment */
            if ($is_admin || isset($roles[$TRANSLATE])) {
                addUpdateComment($mysqli, $_REQUEST['update_id'], $ui_lang_id, $_REQUEST['comment']);
            }
        }
    }
    /* redirect to remove params */
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    $url = strtok($url, '?');
    header("Location: https://{$url}?id=$_REQUEST[id]");
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
    <script>
        function showUpdate() {
            document.getElementById('subject_edit').style.display = '';
            document.getElementById('subject_noedit').style.display = 'none';
            document.getElementById('project_edit').style.display = '';
            document.getElementById('project_noedit').style.display = 'none';
            document.getElementById('assignee_edit').style.display = '';
            document.getElementById('assignee_noedit').style.display = 'none';
            document.getElementById('priority_edit').style.display = '';
            document.getElementById('priority_noedit').style.display = 'none';
            document.getElementById('status_edit').style.display = '';
            document.getElementById('status_noedit').style.display = 'none';
            document.getElementById('due_edit').style.display = '';
            document.getElementById('due_noedit').style.display = 'none';
            document.getElementById('description_edit').style.display = '';
            document.getElementById('description_noedit').style.display = 'none';
            document.getElementById('comment_edit').style.display = '';
            document.getElementById('button_edit').style.display = '';
            document.getElementById('buttons').style.display = 'none';
        }
        function hideUpdate() {
            document.getElementById('subject_edit').style.display = 'none';
            document.getElementById('subject_noedit').style.display = '';
            document.getElementById('project_edit').style.display = 'none';
            document.getElementById('project_noedit').style.display = '';
            document.getElementById('assignee_edit').style.display = 'none';
            document.getElementById('assignee_noedit').style.display = '';
            document.getElementById('priority_edit').style.display = 'none';
            document.getElementById('priority_noedit').style.display = '';
            document.getElementById('status_edit').style.display = 'none';
            document.getElementById('status_noedit').style.display = '';
            document.getElementById('due_edit').style.display = 'none';
            document.getElementById('due_noedit').style.display = '';
            document.getElementById('description_edit').style.display = 'none';
            document.getElementById('description_noedit').style.display = '';
            document.getElementById('comment_edit').style.display = 'none';
            document.getElementById('button_edit').style.display = 'none';
            document.getElementById('buttons').style.display = '';
        }
        function showTranslate() {
            document.getElementById('subject_edit').style.display = '';
            document.getElementById('description_edit').style.display = '';
            document.getElementById('button_translate').style.display = '';
            document.getElementById('buttons').style.display = 'none';
        }
        function hideTranslate() {
            document.getElementById('subject_edit').style.display = 'none';
            document.getElementById('description_edit').style.display = 'none';
            document.getElementById('button_translate').style.display = 'none';
            document.getElementById('buttons').style.display = '';
        }
        function showTransUpdate(btn, input) {
            document.getElementById(btn).style.display = 'none';
            document.getElementById(input).style.display = '';
        }
        function hideTransUpdate(btn, input) {
            document.getElementById(btn).style.display = '';
            document.getElementById(input).style.display = 'none';
        }
    </script>
</head>
<body>
<?php include('nav.php'); ?>
<?php if (count($issue) == 0): ?>
    <ul class="breadcrumbs">
        <li><a href="index.php">Dashboard</a></li>
        <li><a href="issues.php">Issue List</a></li>
        <li class="current">Invalid Issue</li>
    </ul>
    <div class="medium-12 columns">
        <h3>Issue #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3>
        <div class="alert-box alert">Issue not found or access denied.</div>
    </div>
<?php else: ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li><a href="projects.php">Project List</a></li>
    <li><a href="project.php?id=<?php echo $issue[0]['proj_id']; ?>"><?php echo htmlspecialchars($issue[0]['proj_name']); ?></a></li>
    <li><a href="issues.php?id=<?php echo $issue[0]['proj_id']; ?>">Issue List</a></li>
    <li class="current">Issue #<?php echo $issue[0]['issue_id']; ?></li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>Issue #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3></li>
        <li id="buttons">
            <ul class="button-group radius">
<?php if (isset($roles[$PROJECT]) || isset($roles[$UPDATE]) || $is_admin): ?>
                <li><button class="button tiny" onclick="showUpdate()">Update</button></li>
<?php endif ?>
<?php if ((isset($roles[$TRANSLATE]) || $is_admin) && ($issue[0]['translated'] == 0 || $outdated != null)): ?>
                <li><button class="button tiny" onclick="showTranslate()">Translate</button></li>
<?php endif ?>
            </ul>
        </li>
<?php if ((isset($roles[$TRANSLATE]) || $is_admin) && $outdated != null): ?>
        <li class="label alert radius">Translation Out of Date</li>
<?php endif ?>
<?php if ($issue[0]['translated'] == 0): ?>
        <li class="label alert radius">Not Translated</li>
<?php endif ?>
    </ul>
    <div class="medium-12 panel clearfix">
        <form action="issue.php" method="POST" data-abide>
            <input type="hidden" name="id" value="<?php echo htmlspecialchars($_REQUEST['id']); ?>">
            <div class="clearfix">
                <div class="medium-2 column"><label for="subject">Subject</label></div>
                <div class="medium-10 column">
                    <div id="subject_noedit"><?php echo htmlspecialchars($issue[0]['subject']); ?></div>
                    <div id="subject_edit" style="display:none;">
                        <input type="text" id="subject" name="subject" value="<?php echo htmlspecialchars($issue[0]['subject']); ?>" required>
                        <small class="error">A subject is required.</small>
                    </div>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="project">Project</label></div>
                <div class="medium-4 column">
                    <div id="project_noedit"><?php echo htmlspecialchars($issue[0]['proj_name']); ?></div>
                    <div id="project_edit" style="display:none;">
                        <select name="project" required>
<?php foreach ($projects as $project): ?>
                            <option value="<?php echo $project['proj_id']; ?>"<?php if ($project['proj_id'] == $issue[0]['proj_id']) {echo "selected";} ?>><?php echo $project['name'] ?></option>
<?php endforeach ?>
                        </select>
                    </div>
                </div>
                <div class="medium-2 column"><label for="created">Created On</label></div>
                <div class="medium-4 column" id="created"><?php echo htmlspecialchars($issue[0]['created_on']); ?>&nbsp;</div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="assignee">Assignee</label></div>
                <div class="medium-4 column">
                    <div id="assignee_noedit"><?php echo htmlspecialchars($issue[0]['assignee_name']); ?>&nbsp;</div>
                    <div id="assignee_edit" style="display:none;">
                        <select name="assignee">
                            <option value="">None</option>
<?php foreach ($proj_users as $user): ?>
                            <option value="<?php echo $user['user_id']; ?>"<?php if ($user['user_id'] == $issue[0]['assignee_id']) {echo "selected";} ?>><?php echo $user['first_name'].' '.$user['last_name'] ?></option>
<?php endforeach ?>
                        </select>
                    </div>
                </div>
                <div class="medium-2 column"><label for="createdby">Created By</label></div>
                <div class="medium-4 column" id="createdby"><?php echo htmlspecialchars($issue[0]['created_by']); ?>&nbsp;</div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="priority">Priority</label></div>
                <div class="medium-4 column">
                    <div id="priority_noedit"><?php echo htmlspecialchars($issue[0]['priority']); ?>&nbsp;</div>
                    <div id="priority_edit" style="display:none;">
                        <select name="priority" required>
<?php foreach ($priorities as $priority): ?>
                            <option value="<?php echo $priority['priority_id']; ?>"<?php if ($priority['priority_id'] == $issue[0]['priority_id']) {echo "selected";} ?>><?php echo $priority['name'] ?></option>
<?php endforeach ?>
                        </select>
                        <small class="error">A priority is required.</small>
                    </div>
                </div>
                <div class="medium-2 column"><label for="updated">Updated On</label></div>
                <div class="medium-4 column" id="updated"><?php echo htmlspecialchars($issue[0]['last_update']); ?>&nbsp;</div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="status">Status</label></div>
                <div class="medium-4 column">
                    <div id="status_noedit"><?php echo htmlspecialchars($issue[0]['status']); ?>&nbsp;</div>
                    <div id="status_edit" style="display:none;">
                        <select name="status" required>
<?php foreach ($statuses as $status): ?>
                            <option value="<?php echo $status['status_id']; ?>"<?php if ($status['status_id'] == $issue[0]['status_id']) {echo "selected";} ?>><?php echo $status['name'] ?></option>
<?php endforeach ?>
                        </select>
                        <small class="error">A status is required.</small>
                    </div>
                </div>
                <div class="medium-2 column"><label for="updatedby">Updated By</label></div>
                <div class="medium-4 column" id="updatedby"><?php echo htmlspecialchars($issue[0]['updated_by']); ?>&nbsp;</div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column"><label for="duedate">Due&nbsp;Date</label></div>
                <div class="medium-4 column">
                    <div id="due_noedit"><?php echo htmlspecialchars($issue[0]['due_date']); ?>&nbsp;</div>
                    <div id="due_edit" class="row collapse" style="display:none;">
                        <div class="medium-6 column"><input id="duedate" name="duedate" type="date" value="<?php if (strlen($issue[0]['due_date']) > 0) {echo date("Y-m-d", strtotime($issue[0]['due_date']));} ?>" placeholder="YYYY-MM-DD"></div>
                        <div class="medium-6 column"><input id="duetime" name="duetime" type="time" value="<?php if (strlen($issue[0]['due_date']) > 0) {echo date("H:i:s", strtotime($issue[0]['due_date']));} ?>" placeholder="HH:MM:SS"></div>
                    </div>
                </div>
                <div class="medium-2 column"><label for="completed">Completed On</label></div>
                <div class="medium-4 column" id="completed"><?php echo htmlspecialchars($issue[0]['completed_on']); ?>&nbsp;</div>
            </div>
            <div class="clearfix">
                <div class="medium-12 large-2 column"><label for="description">Description</label> </div>
                <div class="medium-12 large-10 column">
                    <div id="description_noedit"><?php echo nl2br(htmlspecialchars($issue[0]['description'])); ?></div>
                    <div id="description_edit" style="display:none;">
                        <textarea id="description" name="description" rows="5" maxlength="2000"><?php echo htmlspecialchars($issue[0]['description']); ?></textarea>
                    </div>
                </div>
            </div>
            <div class="clearfix" id="comment_edit" style="display:none;">
                <hr>
                <div class="medium-12 large-2 column"><label for="comment">Update Comment</label></div>
                <div class="medium-12 large-10 column">
                    <textarea id="comment" name="comment" rows="5" maxlength="2000"></textarea>
                </div>
            </div>
            <div class="clearfix" id="button_edit" style="display:none;">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" class="button tiny" name="action" value="update">Update</button></li>
                        <li><button class="button tiny" onclick="hideUpdate();return false;">Cancel</button></li>
                    </ul>
                </div>
            </div>
            <div class="clearfix" id="button_translate" style="display:none;">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" class="button tiny" name="action" value="translate">Save Translation</button></li>
                        <li><button class="button tiny" onclick="hideTranslate();return false;">Cancel</button></li>
                    </ul>
                </div>
            </div>
        </form>
        <div><hr></div>
        <ul class="inline-list" style="margin-bottom: 0;">
            <li style="margin-left: 0;"><h4>Related Issues</h4></li>
            <li><button onclick="document.getElementById('add_rel').style.display='';this.style.display='none';" class="button radius tiny">Add</button></li>
        </ul>
<?php if (count($rel_issues) > 0): ?>
        <table>
            <thead>
            <tr>
                <th>Relation</th>
                <th>Issue #</th>
                <th>Subject</th>
                <th>Status</th>
                <th></th>
            </tr>
            </thead>
            <tbody>
<?php foreach ($rel_issues as $rel): ?>
<?php $rel_issue = getIssues($mysqli, $user_id, $ui_lang_id, $issue_sort_col, $issue_sort_dir, $rel['rel_issue_id']); ?>
            <tr>
                <td><?php echo $rel['descrip'] ?></td>
                <td><a href="issue.php?id=<?php echo $rel['rel_issue_id'] ?>"><?php if ($rel_issue[0]['complete'] == 1) {echo "<s>";} echo $rel['rel_issue_id']; if ($rel_issue[0]['complete'] == 1) {echo "</s>";} ?></a></td>
                <td><a href="issue.php?id=<?php echo $rel['rel_issue_id'] ?>"><?php if ($rel_issue[0]['complete'] == 1) {echo "<s>";} echo $rel_issue[0]['subject'].($rel_issue[0]['translated'] ? "" : " (Not Translated)"); if ($rel_issue[0]['complete'] == 1) {echo "</s>";} ?></a></td>
                <td><?php echo $rel_issue[0]['status'] ?></td>
                <td><a href="issue.php?id=<?php echo $rel['issue_id'] ?>&rel_id=<?php echo $rel['rel_issue_id'] ?>&action=del_rel" title="Remove Related Issue"><i class="fi-trash"></i></a></td>
            </tr>
<?php endforeach ?>
            </tbody>
        </table>
<?php endif ?>
        <div id="add_rel" class="clearfix" style="display:none;">
            <form action="issue.php" method="POST" data-abide>
                <input type="hidden" name="id" value="<?php echo $_REQUEST['id']; ?>">
                <div class="medium-3 column">
                    <label for="rel_type">Relationship</label>
                    <select name="rel_type" id="rel_type">
<?php foreach ($rels as $rel): ?>
                            <option value="f_<?php echo $rel['rel_id']; ?>"><?php echo $rel['forward_desc']; ?></option>
                            <option value="r_<?php echo $rel['rel_id']; ?>"><?php echo $rel['reverse_desc']; ?></option>
<?php endforeach ?>
                    </select>
                </div>
                <div class="medium-3 column">
                    <label for="rel_issue_id">Issue #</label>
                    <input type="number" id="rel_issue_id" list="issues" name="issue_id" required>
                    <small class="error">You must enter a valid issue number.</small>
                    <datalist id="issues">
<?php foreach ($issues as $i): ?>
<?php if ($i['issue_id'] != $_REQUEST['id'] && !hasId($rel_issues, $i['issue_id'])): ?>
                        <option value="<?php echo $i['issue_id']; ?>"><?php echo $i['issue_id'].': '.$i['subject']; ?></option>
<?php endif ?>
<?php endforeach ?>
                    </datalist>
                </div>
                <div class="medium-6 column">
                    <label>&nbsp;</label>
                    <ul class="button-group radius bottom">
                        <li><button class="button tiny" type="submit" name="action" value="add_rel"><i class="fi-plus"></i></button></li>
                    </ul>
                </div>
            </form>
        </div>
    </div>
<?php if (count($updates) > 0): ?>
    <div class="row">
        <h4>Updates</h4>
<?php $i = count($updates); foreach ($updates as $update): ?>
        <ul class="pricing-table">
            <li class="title text-left">
                <ul class="inline-list" style="margin-bottom:0;">
                    <li><?php echo '#'.$i--; ?></li>
<?php if ($update['translated'] == 0): ?>
                    <li class="label alert radius right">Not Translated</li>
<?php endif ?>
                </ul>

            </li>
<?php if (strlen($update['comment']) > 0): ?>
            <li class="bullet-item text-left clearfix">
                <div class="row">
                    <div class="medium-10 column" style="padding-bottom:.5em;"><?php echo nl2br(htmlspecialchars($update['comment'])); ?></div>
<?php if ($update['translated'] == 0 && (isset($roles[$TRANSLATE]) || $is_admin)): ?>
                    <div class="medium-2 column" id="btn_<?php echo $update['update_no']; ?>"><button class="button radius tiny" onclick="showTransUpdate('btn_<?php echo $update['update_no']; ?>', 'trans_<?php echo $update['update_no']; ?>')">Translate</button></div>
<?php endif ?>
                </div>
<?php if ($update['translated'] == 0 && (isset($roles[$TRANSLATE]) || $is_admin)): ?>
                <div id="trans_<?php echo $update['update_no']; ?>" style="display:none;">
                    <form action="issue.php" method="POST">
                        <input type="hidden" name="id" value="<?php echo $_REQUEST['id']; ?>">
                        <input type="hidden" name="update_id" value="<?php echo $update['update_id']; ?>">
                        <div class="row">
                            <div class="medium-12 column">
                                <textarea name="comment" rows="5" maxlength="2000"><?php echo htmlspecialchars($update['comment']); ?></textarea>
                            </div>
                        </div>
                        <div class="row">
                            <div class="medium-12 column">
                                <ul class="button-group radius">
                                    <li><button type="submit" class="button tiny" name="action" value="trans_update">Save Translation</button></li>
                                    <li><button class="button tiny" onclick="hideTransUpdate('btn_<?php echo $update['update_no']; ?>', 'trans_<?php echo $update['update_no']; ?>');return false;">Cancel</button></li>
                                </ul>
                            </div>
                        </div>
                    </form>
                </div>
<?php endif ?>
            </li>
<?php endif ?>
            <li class="description">Updated by <?php echo $update['full_name'] ?> on <?php echo $update['created_on'] ?></li>
        </ul>
<?php endforeach ?>
    </div>
<?php endif ?>
</div>
<?php endif ?>
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
