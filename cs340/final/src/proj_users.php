<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

/**
 * Adds a user to a project with a specific role.
 * @param $db
 * @param $proj_id
 * @param $add_id
 * @param $role_id
 */
function addProjectUser($db, $proj_id, $add_id, $role_id) {
    $query = "INSERT INTO Users_Projects (user_id, proj_id, role) VALUES(?, ?, ?);";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iii", $add_id, $proj_id, $role_id);
    executeStatement($stmt);
}

/**
 * Removes the specified user from the specified role in the specified project.
 * @param $db
 * @param $proj_id
 * @param $remove_id
 * @param $role
 */
function removeProjectUser($db, $proj_id, $remove_id, $role) {
    $query = "DELETE FROM Users_Projects WHERE proj_id = ? AND user_id = ? AND role = ?;";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "iii", $proj_id, $remove_id, $role);
    executeStatement($stmt);
}

/* get UI languages */
$languages = getLanguages($mysqli);

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
    echo "lang request handled";
}

/* check if project ID was specified */
if (!isset($_REQUEST['id'])) {
    /* no project ID specified--redirect to list of all projects */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    header("Location: https://{$host}{$url}/projects.php");
    die();
}

/* for permissions checking */
$is_admin = isAdmin($mysqli, $user_id);
$user_roles = getProjectRoles($mysqli, $user_id, $_REQUEST['id']);

if (isset($_REQUEST['action']) && isset($_REQUEST['id'])) {
    if ($_REQUEST['action'] == 'remove') {
        if ($is_admin || isset($user_roles[$PROJECT])) {
            removeProjectUser($mysqli, $_REQUEST['id'], $_POST['user_id'], $_POST['role']);
        }
    } else if ($_REQUEST['action'] == 'add') {
        if ($is_admin || isset($user_roles[$PROJECT])) {
            addProjectUser($mysqli, $_REQUEST['id'], $_POST['user_id'], $_POST['role']);
        }
    }
    /* redirect to avoid problems with back button */
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    header("Location: https://$url?id=$_REQUEST[id]");
    die();
}

$users = getAllUsers($mysqli);
$proj_users = getProjectUsersRoles($mysqli, $user_id, $_REQUEST['id']);
$project = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir, $_REQUEST['id']);

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Issue Tracker - Project # <?php echo htmlspecialchars($_REQUEST['id']); ?> User List</title>
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
    <li><a href="project.php?id=<?php echo $project[0]['proj_id']; ?>"><?php echo htmlspecialchars($project[0]['name']); ?></a></li>
    <li class="current">Project Users</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom:0;">
        <li><h3>Project Users</h3></li>
    </ul>
    <div class="medium-12 clearfix">
        <div class="clearfix">
            <div class="medium-3 large-2 column" style="padding-left:0;"><strong>Username</strong></div>
            <div class="medium-4 large-3 column"><strong>Name</strong></div>
            <div class="medium-2 large-2 column"><strong>Role</strong></div>
            <div class="medium-3 large-5 column"></div>
        </div>
<?php foreach ($proj_users as $pu): ?>
        <div class="clearfix">
            <div class="medium-3 large-2 column" style="border:1px solid #ddd;line-height:2em;"><?php echo $pu['username']; ?></div>
            <div class="medium-4 large-3 column" style="border:1px solid #ddd;line-height:2em;"><?php echo $pu['first_name'].' '.$pu['last_name']; ?></div>
            <div class="medium-2 large-2 column" style="border:1px solid #ddd;line-height:2em;"><?php echo $role_map[$pu['role']]; ?></div>
<?php if ($is_admin || isset($user_roles[$PROJECT])): ?>
                <div class="medium-3 large-5 column">
                    <form action="proj_users.php" method="POST">
                        <input type="hidden" name="id" value="<?php echo $project[0]['proj_id']; ?>">
                        <input type="hidden" name="user_id" value="<?php echo $pu['user_id']; ?>">
                        <input type="hidden" name="role" value="<?php echo $pu['role']; ?>">
                        <ul class="button-group radius">
                            <li><button type="submit" name="action" value="remove" title="Remove User" class="button tiny"><i class="fi-x"></i></button></li>
                        </ul>
                    </form>
                </div>
<?php endif ?>
        </div>
<?php endforeach ?>
<?php if ($is_admin || isset($user_roles[$PROJECT])): ?>
    <hr style="margin-top: 0;">
    <div class="clearfix">
        <h4>Add User</h4>
        <form action="proj_users.php" method="POST">
            <input type="hidden" name="id" value="<?php echo $project[0]['proj_id']; ?>">
            <div class="medium-4 large-3 column" style="padding-left:0;">
                <select name="user_id" required>
                    <option value="" disabled selected>Select Username</option>
<?php foreach ($users as $user): ?>
                    <option value="<?php echo $user['user_id']; ?>"><?php echo $user['username'].' ('.$user['first_name'].' '.$user['last_name'].')'; ?></option>
<?php endforeach ?>
                </select>
            </div>
            <div class="medium-4 large-3 column">
                <select name="role" required>
                    <option value="" disabled selected>Select Role</option>
                    <option value="<?php echo $READ; ?>"><?php echo $role_map[$READ]; ?></option>
                    <option value="<?php echo $UPDATE; ?>"><?php echo $role_map[$UPDATE]; ?></option>
                    <option value="<?php echo $CREATE; ?>"><?php echo $role_map[$CREATE]; ?></option>
                    <option value="<?php echo $TRANSLATE; ?>"><?php echo $role_map[$TRANSLATE]; ?></option>
                    <option value="<?php echo $PROJECT; ?>"><?php echo $role_map[$PROJECT]; ?></option>
                </select>
            </div>
            <div class="medium-4 large-6 column">
                <ul class="button-group radius">
                    <button type="submit" name="action" value="add" class="button tiny" title="Add User"><i class="fi-plus"></i></button>
                </ul>
            </div>
        </form>
    </div>
<?php endif ?>
</div>
<?php endif ?>
</div>
</body>
</html>