<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'session.php';
include 'dbfuncs.php';
include 'uifuncs.php';

function addProject($db, $user_id, $proj_name, $proj_desc) {
    $query = "INSERT INTO Projects (name, description) ".
             "SELECT ?, ? FROM Users WHERE user_id = $user_id AND admin = 1";
    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ss", $proj_name, $proj_desc);
    executeStatement($stmt);
    return $stmt->insert_id;
}

/* check for URL params */
if (isset($_REQUEST['lang'])) {
    handleLangRequest($mysqli, $user_id);
} /* sort issue list */
else if (isset($_REQUEST['sort'])) {
    /* reverse if col is the same */
    if ($proj_sort_col == $_REQUEST['sort']) {
        $proj_sort_dir = $proj_sort_dir == 'ASC' ? 'DESC' : 'ASC';
        $_SESSION['proj_sort_dir'] = $proj_sort_dir;
    } /* input validation */
    else if ($_REQUEST['sort'] > 0 && $_REQUEST['sort'] < 4) {
        $proj_sort_col = $_REQUEST['sort'];
        $_SESSION['proj_sort_col'] = $proj_sort_col;
    }

    // redirect to remove URL parameters
    $url = $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
    $url = strtok($url, '?');
    header("Location: https://$url");
    die();
} /* check if adding new project */
else if (isset($_REQUEST['action']) && $_REQUEST['action'] == 'add') {
    $newId = addProject($mysqli, $user_id, $_REQUEST['proj_name'], $_REQUEST['proj_desc']);
    $url = $_SERVER['HTTP_HOST'].rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    if (!is_null($newId) && $newId > 0) {
        $url .= "/project.php?id=$newId";
    } else {
        $url .= "/projects.php";
    }
    header("Location: https://$url");
    die();
}

/* get UI languages */
$languages = getLanguages($mysqli);
/* get project list */
$projects = getProjects($mysqli, $user_id, $proj_sort_col, $proj_sort_dir);
/* permission checking */
$is_admin = isAdmin($mysqli, $user_id);

$sort_arrow = $proj_sort_dir == 'ASC' ? '<i class="fi-arrow-up"></i>' : '<i class="fi-arrow-down"></i>';

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
    <script>
        function showNew() {
            document.getElementById('new_proj').style.display = '';
            document.getElementById('new_button').style.display = 'none';
        }
        function endNew() {
            document.getElementById('new_proj').style.display = 'none';
            document.getElementById('new_button').style.display = '';
        }
    </script>
</head>
<body>
<?php include('nav.php'); ?>
<ul class="breadcrumbs">
    <li><a href="index.php">Dashboard</a></li>
    <li class="current">Project List</li>
</ul>
<div class="medium-12 columns">
    <ul class="inline-list" style="margin-bottom: 0;">
        <li style="margin-left: 0;"><h3>All Projects</h3></li>
<?php if ($is_admin): ?>
        <li id="new_button"><button class="button radius tiny" onclick="showNew()">New Project</button></li>
<?php endif ?>
    </ul>
    <div class="medium-12 panel" id="new_proj" style="display:none;">
        <form action="projects.php" method="POST" data-abide>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="proj_name">Name</label>
                </div>
                <div class="medium-10 column">
                    <input type="text" id="proj_name" name="proj_name" placeholder="Enter the name of the project." required>
                    <small class="error">A project name is required.</small>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-2 column">
                    <label for="proj_desc">Description</label>
                </div>
                <div class="medium-10 column">
                    <textarea id="proj_desc" name="proj_desc" rows="5" maxlength="2000" placeholder="Enter a description of the project. (Optional)"></textarea>
                </div>
            </div>
            <div class="clearfix">
                <div class="medium-12 column text-center">
                    <ul class="button-group radius">
                        <li><button type="submit" name="action" value="add" class="button tiny">Add</button></li>
                        <li><button class="button tiny" onclick="endNew();return false;">Cancel</button></li>
                    </ul>
                </div>
            </div>
        </form>
    </div>
    <table>
        <thead>
        <tr>
            <th><a href="?sort=1" title="Sort by Project #">Project&nbsp;#&nbsp;<?php echo ($proj_sort_col == 1 ? $sort_arrow : ''); ?></a></th>
            <th><a href="?sort=2" title="Sort by Project Name">Name&nbsp;<?php echo ($proj_sort_col == 2 ? $sort_arrow : ''); ?></a></th>
            <th><a href="?sort=3" title="Sort by Project Description">Description&nbsp;<?php echo ($proj_sort_col == 3 ? $sort_arrow : ''); ?></a></th>
        </tr>
        </thead>
        <tbody>
        <?php if (count($projects) > 0): ?>
            <?php foreach ($projects as $project): ?>
                <tr>
                    <td><a href="project.php?id=<?php echo $project['proj_id']; ?>"><?php echo $project['proj_id']; ?></a></td>
                    <td><a href="project.php?id=<?php echo $project['proj_id']; ?>"><?php echo htmlspecialchars($project['name']); ?></a></td>
                    <td><?php if (strlen($project['description']) > 200) {echo htmlspecialchars(substr($project['description'], 0, 200)."..."); } else {echo htmlspecialchars($project['description']);} ?></td>
                </tr>
            <?php endforeach ?>
        <?php else: ?>
            <tr>
                <td colspan="7">You do not have access to any projects.</td>
            </tr>
        <?php endif ?>
        </tbody>
    </table>
</div>
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