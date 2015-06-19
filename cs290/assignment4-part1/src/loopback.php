<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

/* declare array for holding result */
$json = array();

/* test if the request was a GET request or a POST request */
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    $request = &$_GET;
    $json['Type'] = 'GET';
} elseif ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $request = &$_POST;
    $json['Type'] = 'POST';
} else {
    echo "Invalid Request Type";
    die();
}

/* create nested array to hold parameters or null if no parameters*/
if (count($request) == 0)
    $json['parameters'] = null;
else
    $json['parameters'] = array();
foreach ($request as $key => $val) {
    $json['parameters'][$key] = $val;
}

/* set content type */
header('Content-Type: application/json');

/* convert the array to JSON and output it */
echo json_encode($json);
?>