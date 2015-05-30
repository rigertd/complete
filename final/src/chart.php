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



function getPercentileData($db, $type, $gender) {
    $stmt = prepareQuery($db, "SELECT months, percentile, val " .
                              "FROM WHOMaster " .
                              "WHERE type = ? AND gender = ? " .
                              "ORDER BY months ASC, percentile DESC, val ASC");
    bindParam($stmt, "ss", $type, $gender);
    executeStatement($stmt);
    $result = $stmt->get_result();
    /* store results by percentile and then by month */
    $arr = array();
    while ($row = $result->fetch_assoc()) {
        /* get percentile as a string with minimum required significant digits */
        $p = rtrim(number_format($row["percentile"], 1, '.', ''), "0");
        $p = rtrim($p, ".");
        $arr[$p][$row["months"]] = number_format($row["val"], 1, '.', '');
    }
    return $arr;
}

if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['action'])) {
    switch($_GET['action']) {
        case "percentile":
            $type = isset($_GET['type']) ? $_GET['type'] : '';
            $gender = isset($_GET['gender']) ? $_GET['gender'] : '';
            header('Content-Type: application/json');
            echo json_encode(getPercentileData($mysqli, $type, $gender));
    }
}


