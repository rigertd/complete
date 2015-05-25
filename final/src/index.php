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
    header("Location: http://{$host}{$url}/login.php");
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
    $stmt = prepareQuery($db, 'SELECT p.id, p.name, p.dob ' .
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
<script src="../js/login.js"></script>
</head>
<body>
  <div class="container">
    <header class="page-header clearfix">
      <h2 class="col-sm-9">Baby Growth Tracker</h2>
      <div class="col-sm-3"><a href="login.php?action=logout" class="btn btn-default" role="button">Log Out</a></div>
    </header>
    <select name="profile">
<?php foreach ($profiles as $profile): ?>
      <option value="<?php echo $profile['id']; ?>"><?php echo htmlspecialchars($profile['name'].' ('.$profile['dob'].')') ?></option>
<?php endforeach ?>
    </select>
    <p>Hello <?php echo $first ?>, welcome to the dashboard.</p>
    <footer>&copy; David Rigert</footer>
  </div>
</body>
</html>