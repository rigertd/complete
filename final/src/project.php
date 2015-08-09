<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

function updateProject($db, $user_id, $proj_id, $proj_name, $proj_desc) {
    global $PROJECT;
    $query = "UPDATE Projects p LEFT JOIN Users_Projects up ON p.proj_id = up.proj_id ".
             "SET p.name = ?, p.description = ? ".
             "WHERE p.proj_id = ? AND ((up.user_id = ? AND up.role = $PROJECT) ".
             "OR EXISTS (SELECT * FROM Users WHERE user_id = ? AND admin = 1));";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ssiii", $proj_name, $proj_desc, $proj_id, $user_id, $user_id);
    executeStatement($stmt);
    return $stmt->affected_rows > 0;
}

/* get UI languages */
$languages = getLanguages($mysqli);

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
}
/* check if project ID was specified */
if (isset($_REQUEST['id'])) {
    if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'edit') {
        /* update project info */
        updateProject($mysqli, $user_id, $_REQUEST['id'], rtrim($_REQUEST['proj_name']), rtrim($_REQUEST['proj_desc']));
        /* redirect to avoid problems with back button */
        $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
        header("Location: https://$url?id=$_REQUEST[id]");
        die();
    } else {
        /* get project info and display it */
        $project = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir, $_REQUEST['id']);
    }
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
    <script>
        function beginEdit() {
            document.getElementById('name_noedit').style.display = 'none';
            document.getElementById('name_edit').style.display = '';
            document.getElementById('desc_noedit').style.display = 'none';
            document.getElementById('desc_edit').style.display = '';
            document.getElementById('edit_buttons').style.display = '';
        }
        function endEdit() {
            document.getElementById('name_noedit').style.display = '';
            document.getElementById('name_edit').style.display = 'none';
            document.getElementById('desc_noedit').style.display = '';
            document.getElementById('desc_edit').style.display = 'none';
            document.getElementById('edit_buttons').style.display = 'none';
        }
    </script>
</head>
<body>
<?php include('nav.php'); ?>
<?php if (count($project) == 0): ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li><a href="projects.php">Project List</a></li>
    <li class="current">Invalid Project</li>
</ul>
<div class="medium-12 columns">
    <h3>Project #<?php echo htmlspecialchars($_REQUEST['id']); ?></h3>
    <div class="alert-box alert">Project not found or access denied.</div>
</div>
<?php else: ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li><a href="projects.php">Project List</a></li>
    <li class="current"><?php echo htmlspecialchars($project[0]['name']); ?></li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom:0;">
        <li><h3>Project Overview</h3></li>
        <li>
            <ul class="button-group radius">
                <li><a href="issues.php?id=<?php echo $project[0]['proj_id']; ?>" class="button tiny">Issues</a></li>
                <li><a href="proj_users.php?id=<?php echo $project[0]['proj_id']; ?>" class="button tiny">Users</a></li>
<?php if (isset($user_roles[$PROJECT]) || $is_admin): ?>
                <li><button class="button tiny" onclick="beginEdit()">Edit</button></li>
<?php endif ?>
            </ul>
        </li>
    </ul>
    <div class="medium-12 panel">
        <form action="project.php" method="POST" data-abide>
            <div class="clearfix">
                <input type="hidden" name="id" value="<?php echo $project[0]['proj_id']; ?>">
                <div class="medium-2 column">
                    <label for="proj_name">Name</label>
                </div>
                <div class="medium-10 column">
                    <div id="name_noedit"><?php echo htmlspecialchars($project[0]['name']); ?></div>
                    <div id="name_edit" style="display:none;">
                        <input type="text" id="proj_name" name="proj_name" value="<?php echo htmlspecialchars($project[0]['name']); ?>" required>
                        <small class="error">A project name is required.</small>
                    </div>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="proj_desc">Description</label>
                </div>
                <div class="medium-10 column">
                    <div id="desc_noedit"><?php echo nl2br(htmlspecialchars($project[0]['description'])); ?></div>
                    <div id="desc_edit" style="display:none;"><textarea id="proj_desc" name="proj_desc" rows="5" maxlength="2000"><?php echo htmlspecialchars($project[0]['description']); ?></textarea></div>
                </div>
            </div>
            <div class="clearfix" id="edit_buttons" style="display:none;">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" name="action" value="edit" class="button tiny">Save</button></li>
                        <li><button class="button tiny" onclick="endEdit();return false;">Cancel</button></li>
                    </ul>
                </div>
            </div>
        </form>
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
