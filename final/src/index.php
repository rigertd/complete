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
    <script src="https://www.google.com/jsapi"></script>
    <script src="../js/tracker.js"></script>
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
            <button type="button" id="profileButton" class="btn btn-default btn-block">Load Chart</button>
          </div>
          <div class="form-group">
            <button type="button" id="deleteButton" class="btn btn-default btn-block">Delete Profile</button>
          </div>
          <div class="form-group">
            <div id="validation_message"></div>
          </div>
        </form>
        <form id="addProfile" action="chart.php">
          <div class="form-group">
            <button type="button" id="newButton" class="btn btn-default btn-block">Add New Profile</button>
          </div>
          <div id="createNew">
            <h4>New Profile</h4>
            <div class="form-group">
              <input type="text" id="profileName" class="form-control" title="Baby's Name" placeholder="Baby's Name" required>
            </div>
            <div class="form-group">
              <input type="text" id="profileDob" class="form-control" pattern="[12][01]\d{2}-[01]\d-[0123]\d" title="Date of birth in YYYY-MM-DD format" placeholder="Date of Birth" required>
            </div>
            <div class="form-group">
              <select id="profileGender" class="form-control">
                <option value="" disabled selected>Gender</option>
                <option value="m">Boy</option>
                <option value="f">Girl</option>
              </select>
            </div>
            <div class="form-group">
              <button type="button" id="addButton" class="btn btn-default btn-block">Save</button>
            </div>
            <div class="form-group">
              <button type="reset" id="cancelButton" class="btn btn-default btn-block">Cancel</button>
            </div>
          </div>
        </form>
      </div>
      <div class="col-sm-9 clearfix">
        <div id="chartDiv"></div>
      </div>
      <footer class="col-sm-12">&copy; David Rigert</footer>
    </div>
  </body>
</html>