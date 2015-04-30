<?php
/* declare array for holding result */
$json = array();

/* test if the request was a GET request or a POST request */
if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    $request = &$_GET;
    $json['Type'] = 'GET';
} else {
    $request = &$_POST;
    $json['Type'] = 'POST';
}

/* create nested array to hold parameters or null if no parameters*/
if (count($request) == 0)
    $json['parameters'] = null;
else
    $json['parameters'] = array();
foreach ($request as $key => $val) {
    $json['parameters'][$key] = $val;
}

/* convert the array to JSON and output it */
echo json_encode($json);
?>