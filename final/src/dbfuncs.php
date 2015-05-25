<?php
/**
 * Stores a collection of wrapper functions for accessing a mysql database.
 * User: David Rigert
 * Date: 5/24/2015
 * Time: 1:16 PM
 */

/* Creates and returns a prepared statement based on the specified mysqli and query */
function prepareQuery($db, $query) {
    if (!($stmt = $db->prepare($query))) {
        echo "Database query error.";
        die();
    }
    return $stmt;
}

/* Binds the specified parameters to the specified prepared statement */
function bindParam($stmt, $type, $arg1, $arg2 = NULL, $arg3 = NULL, $arg4 = NULL, $arg5 = NULL) {
    $numargs = func_num_args();
    $numtypes = strlen($type);

    if ($numargs <= 2) {
        echo "Invalid number of arguments in bindParam. ($numargs)";
        die();
    } else if ($numtypes != ($numargs - 2)) {
        echo "Invalid number of types in \$type."
            . "$numtypes types specified for "
            . ($numargs - 2) . " arguments.";
        die();
    }
    switch ($numargs) {
        case 3:
            if (!($stmt->bind_param($type, $arg1))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 4:
            if (!($stmt->bind_param($type, $arg1, $arg2))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 5:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 6:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3, $arg4))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 7:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3, $arg4, $arg5))) {
                echo "Database binding error.";
                die();
            }
            break;
        default:
            echo "Invalid number of arguments in bindParam. ($numargs)";
            die();
    }
}

/* Executes the specified prepared statement */
function executeStatement($stmt) {
    if (!$stmt->execute()) {
        echo "Error executing operation.";
        die();
    }
}

function getSingleResult($stmt) {
    $result = $stmt->get_result();
    return mysqli_fetch_assoc($result);
}
