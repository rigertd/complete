<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');
require_once('dbinfo.php');

include 'dbfuncs.php';

/* conversion constants */
define('IN_TO_CM', 2.54);
define('LBS_TO_KG', 0.453592);

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

function getWhoResults($db, $type, $pid) {
    $stmt = prepareQuery($db, "SELECT w.months, w.val " .
                              "FROM WHOMaster AS w INNER JOIN Profiles AS p " .
                              "ON w.gender = p.gender AND w.type = ? AND p.id = ? " .
                              "ORDER BY w.months ASC, w.percentile DESC, w.val ASC");
    bindParam($stmt, "si", $type, $pid);
    executeStatement($stmt);
    return $stmt->get_result();

}

function getChartData($db, $type, $pid, $email) {
    $userArr = getUserData($db, $type, $pid, $email);
    $whoResults = getWhoResults($db, $type, $pid);

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
    return count($arr) == 3 && checkdate($arr[1], $arr[2], $arr[0]);
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

/**
 * Adds or updates biometric data in the specified profile.
 * @param mysqli $db          The mysqli database interface.
 * @param String $email       The current user's email address.
 * @param String $pid         The profile ID to add the data to.
 * @param String $months      The child age in months.
 * @param String $length      The child length (height).
 * @param String $lengthUnit  The unit of the length value.
 * @param String $weight      The child weight.
 * @param String $weightUnit  The unit of the weight value.
 * @param String $head        The child head circumference.
 * @param String $headUnit    The unit of the head circumference value.
 * @return mixed Whether the operation succeeded and relevant messages.
 */
function addOrUpdateData($db, $email, $pid, $months, $length, $lengthUnit, $weight, $weightUnit, $head, $headUnit) {
    $obj = array('message' => '',
                 'success' => 'true');

    /* validate data and return error message if invalid */
    if (empty($email)) {
        $obj['message'] = $obj['message'] . 'Invalid user. Try logging in again. ';
    }
    if (empty($pid)) {
        $obj['message'] = $obj['message'] . 'You must select a profile. ';
    }
    if (empty($months)) {
        $obj['message'] = $obj['message'] . 'You must specify the age in months at the time the data was measured. ';
    } else if (!ctype_digit($months) || $months < 0 || $months > 24) {
        $obj['message'] = $obj['message'] . 'You must specify an age between 0 and 24 months. ';
    }
    if (empty($length) && empty($weight) && empty($head)) {
        $obj['message'] = $obj['message'] . 'You must specify at least one measurement. ';
    }
    if (!empty($length) && (!is_numeric($length) || $length < 0 || empty($lengthUnit))) {
        $obj['message'] = $obj['message'] . 'The length must be a positive numeric value. ';
    }
    if (!empty($weight) && (!is_numeric($weight) || $weight < 0 || empty($weightUnit))) {
        $obj['message'] = $obj['message'] . 'The weight must be a positive numeric value. ';
    }
    if (!empty($head) && (!is_numeric($head) || $head < 0 || empty($headUnit))) {
        $obj['message'] = $obj['message'] . 'The head circumference must be a positive numeric value. ';
    }
    if (!empty($obj['message'])) {
      $obj['success'] = 'false';
      return $obj;
    }
    $query = "INSERT INTO Entries (profile_id, type, months, val) ".
             "SELECT p.id, ?, ?, ? FROM Profiles AS p ".
             "INNER JOIN Users AS u ON u.id = p.parent_id AND u.email = ? AND p.id = ? ".
             "ON DUPLICATE KEY UPDATE val = ?";

    $stmt = prepareQuery($db, $query);
    /* run query sequentially on all non-empty values */
    if (!empty($length)) {
        if ($lengthUnit == 'in') {
            $length = $length * IN_TO_CM;
        }
        bindParam($stmt, "sidsid", "length", $months, $length, $email, $pid, $length);
        executeStatement($stmt);
        if ($stmt->affected_rows < 1) {
            $obj['message'] .= $obj['message'] . "Unable to add length value. ";
            $obj['success'] = "false";
        } else if ($stmt->affected_rows == 1) {
            $obj['message'] .= $obj['message'] . "Added a new length value. ";
        } else {
            $obj['message'] .= $obj['message'] . "Updated the existing length value. ";
        }
    }
    if (!empty($weight)) {
        if ($weightUnit == 'lb') {
          $weight = $weight * LBS_TO_KG;
        }
        bindParam($stmt, "sidsid", "weight", $months, $weight, $email, $pid, $weight);
        executeStatement($stmt);
        if ($stmt->affected_rows < 1) {
            $obj['message'] .= $obj['message'] . "Unable to add weight value. ";
            $obj['success'] = "false";
        } else if ($stmt->affected_rows == 1) {
            $obj['message'] .= $obj['message'] . "Added a new weight value. ";
        } else {
            $obj['message'] .= $obj['message'] . "Updated the existing weight value. ";
        }
    }
    if (!empty($head)) {
        if ($headUnit == 'in') {
          $head = $head * IN_TO_CM;
        }
        bindParam($stmt, "sidsid", "head", $months, $head, $email, $pid, $head);
        executeStatement($stmt);
        if ($stmt->affected_rows < 1) {
            $obj['message'] .= $obj['message'] . "Unable to add head circumference value. ";
            $obj['success'] = "false";
        } else if ($stmt->affected_rows == 1) {
            $obj['message'] .= $obj['message'] . "Added a new head circumference value. ";
        } else {
            $obj['message'] .= $obj['message'] . "Updated the existing head circumference value. ";
        }
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
        case "profiles":
            header('Content-Type: application/json');
            echo json_encode(getProfiles($mysqli, $email));
            break;
        case "data":
            $pid = isset($_GET['profile']) ? $_GET['profile'] : null;
            $mo = isset($_GET['mo']) ? $_GET['mo'] : null;
            $l = isset($_GET['l']) ? $_GET['l'] : null;
            $lu = isset($_GET['lu']) ? $_GET['lu'] : null;
            $w = isset($_GET['w']) ? $_GET['w'] : null;
            $wu = isset($_GET['wu']) ? $_GET['wu'] : null;
            $hc = isset($_GET['hc']) ? $_GET['hc'] : null;
            $hcu = isset($_GET['hcu']) ? $_GET['hcu'] : null;
            header('Content-Type: application/json');
            echo json_encode(addOrUpdateData($mysqli, $email, $pid, $mo, $l, $lu, $w, $wu, $hc, $hcu));
            break;
        default:
            echo "Unknown action.";
            die();
    }
}


