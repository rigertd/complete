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
    header("Location: http://{$host}{$url}/login.php?");
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

function getGender($db, $pid, $email) {
    $stmt = prepareQuery($db, "SELECT p.gender ".
                              "FROM Profiles AS p ".
                              "INNER JOIN Users AS u ON p.parent_id = u.id ".
                              "WHERE p.id = ? AND u.email = ?");
    bindParam($stmt, "is", $pid, $email);
    executeStatement($stmt);
    $result = getSingleResult($stmt);
    return isset($result['gender']) ? $result['gender'] : '';
}

function getUserData($db, $type, $pid, $email) {
    $stmt = prepareQuery($db, "SELECT months, val FROM Entries ".
                              "INNER JOIN Profiles AS p ON profile_id = p.id ".
                              "INNER JOIN Users AS u ON p.parent_id = u.id ".
                              "WHERE type = ? ".
                              "AND p.id = ? AND u.email = ? ".
                              "ORDER BY months ASC");
    bindParam($stmt, "sis", $type, $pid, $email);
    executeStatement($stmt);
    $result = $stmt->get_result();
    $arr = array();
    while ($row = $result->fetch_assoc()) {
        $arr[$row['months']] = number_format($row["val"], 1, '.', '');
    }
    return $arr;
}

function getWhoResults($db, $type, $gender) {
    $stmt = prepareQuery($db, "SELECT months, val " .
                              "FROM WHOMaster " .
                              "WHERE type = ? AND gender = ? " .
                              "ORDER BY months ASC, percentile DESC, val ASC");
    bindParam($stmt, "ss", $type, $gender);
    executeStatement($stmt);
    return $stmt->get_result();

}

function getChartData($db, $type, $pid, $email) {
    $gender = getGender($db, $pid, $email);
    $userArr = getUserData($db, $type, $pid, $email);
    $whoResults = getWhoResults($db, $type, $gender);

    /* store WHO results by month */
    /* Columns: Months, 99.9th, 99th, 97th, ..., 3rd, 1st, 0.1th*/
    $arr = array();
    while ($row = $whoResults->fetch_assoc()) {
        if (!isset($arr[$row["months"]])) {
            $arr[$row["months"]] = array($row["months"]);
        }
        $arr[$row["months"]][] = number_format($row["val"], 1, '.', '');
    }

    /* append user data to new array column */
    foreach ($arr as $month => &$row) {
        $row[] = isset($userArr[$month]) ? $userArr[$month] : null;
    }
    return $arr;
}

function validateDate($date) {
    $arr = explode('-', $date);
    return count($arr) == 3 && checkdate($arr[0], $arr[1], $arr[2]);
}

function removeProfile($db, $email, $pid) {
    $obj = array('message' => '');
    /* validate data and return error message if invalid */
    if (empty($email)) {
        $obj['message'] = $obj['message'] . 'Invalid user. Try logging in again. ';
    }
    /* validate data and return error message if invalid */
    if (empty($pid)) {
        $obj['message'] = $obj['message'] . 'You must select the profile to remove. ';
    }
    if (!empty($obj['message'])) {
        $obj['success'] = 'false';
        return $obj;
    }

    $query = "DELETE p FROM Profiles AS p ".
             "INNER JOIN Users AS u ON p.parent_id = u.id ".
             "AND p.id = ? AND u.email = ?";

    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "is", $pid, $email);
    executeStatement($stmt);

    if ($stmt->affected_rows > 0) {
        $obj['message'] = 'Profile successfully deleted.';
        $obj['success'] = 'true';
    } else {
        $obj['message'] = 'Failed to delete the profile.';
        $obj['success'] = 'false';
    }
    return $obj;
}

function addNewProfile($db, $email, $name, $dob, $gender) {
    $obj = array('message' => '');

    /* validate data and return error message if invalid */
    if (empty($email)) {
        $obj['message'] = $obj['message'] . 'Invalid user. Try logging in again. ';
    }
    if (empty($name)) {
        $obj['message'] = $obj['message'] . 'The baby name cannot be blank. ';
    }
    if (empty($dob) || !validateDate($dob)) {
        $obj['message'] = $obj['message'] . 'You must enter a valid date of birth in the YYYY-MM-DD format. ';
    }
    if (empty($gender) || ($gender != 'm' && $gender != 'f')) {
        $obj['message'] = $obj['message'] . 'Invalid gender specified. ';
    }
    if (!empty($obj['message'])) {
        $obj['success'] = 'false';
        return $obj;
    }
    $query = "INSERT INTO Profiles (parent_id, name, gender, dob) ".
             "SELECT id, ?, ?, ? ".
             "FROM Users WHERE email = ?";

    $stmt = prepareQuery($db, $query);
    bindParam($stmt, "ssss", $name, $gender, $dob, $email);
    executeStatement($stmt);

    if ($stmt->affected_rows > 0) {
        $obj['message'] = 'Created a new profile for '.$name.'.';
        $obj['success'] = 'true';
    } else {
        $obj['message'] = 'Failed to create a profile for '.$name.'.';
        $obj['success'] = 'false';
    }
    return $obj;
}

if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['action'])) {
    switch($_GET['action']) {
        case "chart":
            $type = isset($_GET['type']) ? $_GET['type'] : null;
            $pid = isset($_GET['profile']) ? $_GET['profile'] : null;
            header('Content-Type: application/json');
            echo json_encode(getChartData($mysqli, $type, $pid, $email));
            break;
        case "new":
            $name = isset($_GET['profileName']) ? $_GET['profileName'] : null;
            $dob = isset($_GET['profileDob']) ? $_GET['profileDob'] : null;
            $gender = isset($_GET['profileGender']) ? $_GET['profileGender'] : null;
            header('Content-Type: application/json');
            echo json_encode(addNewProfile($mysqli, $email, $name, $dob, $gender));
            break;
        case "delete":
            $pid = isset($_GET['profile']) ? $_GET['profile'] : null;
            header('Content-Type: application/json');
            echo json_encode(removeProfile($mysqli, $email, $pid));
            break;

    }
}


