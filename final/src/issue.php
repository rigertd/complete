<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

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

function getRelatedIssues($db, $user_id, $issue_id) {
    $query = "SELECT ir.issue_id1 AS issue_id, ir.issue_id2 AS rel_issue_id, ir.rel_id, r.forward_desc AS descrip, 'f' AS dir ".
             "FROM Issues_Relationships ir ".
             "INNER JOIN Relationships r ON ir.rel_id = r.rel_id ".
             "INNER JOIN Issues i ON ir.issue_id1 = i.issue_id ".
             "INNER JOIN Users_Projects up ON i.proj_id = up.proj_id ".
             "WHERE ir.issue_id1 = ? AND up.user_id = ? ".
             "UNION ".
             "SELECT ir.issue_id2 AS issue_id, ir.issue_id1 AS rel_issue_id, ir.rel_id, r.reverse_desc AS descrip, 'r' AS dir ".
             "FROM Issues_Relationships ir ".
             "INNER JOIN Relationships r ON ir.rel_id = r.rel_id ".
             "INNER JOIN Issues i ON ir.issue_id2 = i.issue_id ".
             "INNER JOIN Users_Projects up ON i.proj_id = up.proj_id ".
             "WHERE ir.issue_id2 = ? AND up.user_id = ?";
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
    if ($is_admin || isset($roles[$PROJECT]) || isset($roles[$UPDATE])) {
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
            <li style="margin-left: 0;"><h4>Related Issues</h4></li>
            <li><button onclick="document.getElementById('add_rel').style.display='';this.style.display='none';" class="button radius tiny">Add</button></li>
        </ul>
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
                <td><a href="issue.php?id=<?php echo $rel['rel_issue_id'] ?>"><?php echo $rel['rel_issue_id']; ?></a></td>
                <td><a href="issue.php?id=<?php echo $rel['rel_issue_id'] ?>"><?php echo $rel_issue[0]['subject'].($rel_issue[0]['translated'] ? "" : " (Not Translated)"); ?></a></td>
                <td><?php echo $rel_issue[0]['status'] ?></td>
                <td><a href="issue.php?id=<?php echo $rel['issue_id'] ?>&rel_id=<?php echo $rel['rel_issue_id'] ?>&action=del_rel" title="Remove Related Issue"><i class="fi-trash"></i></a></td>
            </tr>
<?php endforeach ?>
            </tbody>
        </table>
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
