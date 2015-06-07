<?php
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
    <script src="../js/cookies.js"></script>
    <script src="../js/tracker.js"></script>
    <script src="../js/draw.js"></script>
  </head>
  <body>
    <div class="container">
      <header class="page-header clearfix">
        <h2 class="col-sm-8">Baby Growth Tracker</h2>
        <div class="col-sm-4 text-right"><span class="h5" style="margin-right:10px;"><?php echo htmlspecialchars("$first $last"); ?></span> <a href="login.php?action=logout" class="btn btn-default" role="button">Sign Out</a></div>
      </header>
      <div class="col-sm-3">
        <form id="selectProfileForm" action="chart.php">
          <input type="hidden" id="gender">
          <div class="form-group">
            <label for="profile" class="sr-only">Profile</label>
            <select id="profile" name="profile" class="form-control">
              <option value="" disabled selected>Select Profile</option>
<?php foreach ($profiles as $profile): ?>
              <option value="<?php echo $profile['id']; ?>"><?php echo htmlspecialchars($profile['name'].' ('.$profile['dob'].')') ?></option>
<?php endforeach ?>
            </select>
          </div>
          <div class="form-group">
            <label for="chartType" class="sr-only">Chart Type</label>
            <select id="chartType" name="type" class="form-control">
              <option value="" disabled selected>Select Chart Type</option>
              <option value="length">Length (Height)</option>
              <option value="weight">Weight</option>
              <option value="head">Head Circumference</option>
            </select>
          </div>
        </form>
        <form id="navButtonForm" action="#">
          <div class="form-group">
            <button type="button" id="addDataButton" class="btn btn-default btn-block">Add Checkup Data</button>
          </div>
          <div class="form-group">
            <button type="button" id="newButton" class="btn btn-default btn-block">Create New Profile</button>
          </div>
          <div class="form-group">
            <button type="button" id="deleteButton" class="btn btn-default btn-block">Delete Profile</button>
          </div>
        </form>
        <form id="addDataForm" action="chart.php">
          <h4>Add or Update Data</h4>
          <div class="form-group">
            <label for="dataMonths" class="sr-only">Age in Months</label>
            <input type="text" id="dataMonths" class="form-control" pattern="\d+" title="Age in Months" placeholder="Age (0-24 months)" required>
          </div>
          <div class="form-group">
            <label for="dataEntry" class="sr-only">Checkup Data</label>
            <input type="text" id="dataEntry" class="form-control" pattern="\d+\.?\d*" title="Checkup Data" placeholder="" required>
            <select id="dataUnit" class="form-control">
              <option value="in">Inches</option>
              <option value="cm">Centimeters</option>
            </select>
          </div>
          <div class="form-group">
            <button id="saveDataButton" type="button" class="btn btn-default btn-block">Save Data</button>
          </div>
          <div class="form-group">
            <button id="cancelDataButton" type="reset" class="btn btn-default btn-block">Cancel</button>
          </div>
        </form>
        <form id="newProfileForm" action="chart.php">
          <h4>New Profile</h4>
          <div class="form-group">
            <label for="profileName" class="sr-only">Baby's Name</label>
            <input type="text" id="profileName" class="form-control" title="Baby's Name" placeholder="Baby's Name" required>
          </div>
          <div class="form-group">
            <label for="profileDob" class="sr-only">Date of Birth</label>
            <input type="text" id="profileDob" class="form-control" pattern="[12][09]\d{2}-[01]\d-[0123]\d" title="Date of birth in YYYY-MM-DD format" placeholder="Birth Date (YYYY-MM-DD)" required>
          </div>
          <div class="form-group">
            <label for="profileGender" class="sr-only">Gender</label>
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
        </form>
        <div class="form-group">
          <div id="validation_message"></div>
        </div>
      </div>
      <div class="col-sm-9 clearfix">
        <div id="welcome">
          <img src="../img/baby_hands.png" class="pull-left img-responsive" alt="A baby's hands">
          <p>Welcome to Baby Growth Tracker, where you can record your baby's length, weight, and head circumference measurements
            from well child visits. Visually compare your baby's growth against the standards set for each measurement
            by <a href="http://www.who.int/childgrowth/standards/en/">the World Health Organization</a>
            on a chart.</p>
          <p>To begin, select a child profile, or create a new profile for your child if you do not already have one.
            Next, select the chart type to view your existing data for that measurement. Click <strong>Add Checkup Data</strong>
            to add new data or update the existing data on the current chart.</p>
        </div>
        <div id="chartDiv"></div>
      </div>
      <footer class="col-sm-12">&copy; David Rigert</footer>
    </div>
  </body>
</html>