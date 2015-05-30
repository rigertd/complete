<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'dbfuncs.php';

session_start();

/* Check if user is logged in */
if (session_status() != PHP_SESSION_ACTIVE || !isset($_SESSION['email'])){
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no username or session--redirect to login.php */
    header("Location: https://{$host}{$url}/login.php");
    die();
}

/* get user info */
$first = $_SESSION['first'];
$last = $_SESSION['last'];
$email = $_SESSION['email'];

/* Establish database connection */
$mysqli = new mysqli(DB_ADDR, DB_USER, DB_PW, DB_NAME);
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/**
 * Gets an array of child profiles associated with the account.
 * @param mysqli $db         The mysqli database interface.
 * @param String $user_email The logged in user's email address.
 * @return array An array of profile {id, name, dob}
 */
function getProfiles($db, $user_email) {
    $stmt = prepareQuery($db, 'SELECT p.id, p.name, p.gender, p.dob ' .
                              'FROM Users as u ' .
                              'INNER JOIN Profiles as p ON u.id = p.parent_id ' .
                              'WHERE u.email = ? ' .
                              'ORDER BY p.name ASC');
    bindParam($stmt, "s", $user_email);
    executeStatement($stmt);
    $results = $stmt->get_result();
    $profiles = array();
    while ($row = $results->fetch_assoc()) {
        $profiles[] = $row;
    }
    return $profiles;
}

$profiles = getProfiles($mysqli, $email);


?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="x-ua-compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Baby Growth Tracker - Dashboard</title>
    <link href="../css/bootstrap.min.css" rel="stylesheet">
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
    <link href="../css/style.css" rel="stylesheet">
    <script src="../js/Chart.min.js"></script>
    <script src="../js/draw.js"></script>
  </head>
  <body>
    <div class="container">
      <header class="page-header clearfix">
        <h2 class="col-sm-9">Baby Growth Tracker</h2>
        <div class="col-sm-3"><?php echo htmlspecialchars("$first $last"); ?> <a href="login.php?action=logout" class="btn btn-default" role="button">Log Out</a></div>
      </header>
      <div class="col-sm-3">
        <form id="profileForm" action="chart.php">
          <input type="hidden" id="gender">
          <div class="form-group">
            <select id="profile" name="profile" class="form-control">
              <option value="" disabled selected>Select Profile</option>
<?php foreach ($profiles as $profile): ?>
              <option value="<?php echo $profile['id']; ?>"><?php echo htmlspecialchars($profile['name'].' ('.$profile['dob'].')') ?></option>
<?php endforeach ?>
            </select>
          </div>
          <div class="form-group">
            <select id="chartType" name="type" class="form-control">
              <option value="" disabled selected>Select Chart Type</option>
              <option value="length">Length (Height)</option>
              <option value="weight">Weight</option>
              <option value="head">Head Circumference</option>
            </select>
          </div>
          <div class="form-group">
            <button type="button" id="profileButton" class="btn btn-default">Load Chart</button>
          </div>
        </form>
      </div>
      <div class="col-sm-9 clearfix">
        <div class="col-sm-10">
          <h4 id="chartTitle">Length (Height) Percentiles</h4>
          <canvas id="chart"></canvas>
        </div>
        <div class="col-sm-2">
          <h4>Legend</h4>
            <ul class="legend">
              <li class="legend p97">97th
              <li class="legend p85">85th
              <li class="legend p50">50th
              <li class="legend p15">15th
              <li class="legend p03">3rd
            </ul>
          </fieldset>
        </div>
      </div>
      <footer>&copy; David Rigert</footer>
    </div>
  </body>
</html>