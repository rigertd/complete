<?php
/**
 * Establishes the database connection.
 * Stores a collection of wrapper functions for accessing a mysql database.
 * User: David Rigert
 * Date: 5/24/2015
 * Time: 1:16 PM
 */

/* Establish database connection */
$mysqli = new mysqli(DB_ADDR, DB_USER, DB_PW, DB_NAME);
$mysqli->set_charset('utf8');
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/**
 * Creates and returns a prepared statement based on the specified mysqli and query.
 * @param mysqli $db    The mysqli instance.
 * @param String $query The SQL query string.
 * @return mysqli_stmt  The resulting mysqli statement.
 */
function prepareQuery($db, $query) {
    if (!($stmt = $db->prepare($query))) {
        echo "Database query error (".$db->errno."). ".$db->error;
        die();
    }
    return $stmt;
}

/**
 * Binds the specified parameters to the specified prepared statement.
 * @param mysqli_stmt $stmt The mysqli statement.
 * @param String $type      The data type of each argument. i=integer, d=double, s=string.
 * @param mixed $arg1       The first argument (required).
 * @param mixed|null $arg2  The second argument (optional).
 * @param mixed|null $arg3  The third argument (optional).
 * @param mixed|null $arg4  The fourth argument (optional).
 * @param mixed|null $arg5  The fifth argument (optional).
 * @param mixed|null $arg6  The sixth argument (optional).
 * @param mixed|null $arg7  The seventh argument (optional).
 */
function bindParam($stmt, $type, $arg1, $arg2 = NULL, $arg3 = NULL, $arg4 = NULL, $arg5 = NULL, $arg6 = NULL, $arg7 = NULL, $arg8 = NULL) {
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
        case 8:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3, $arg4, $arg5, $arg6))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 9:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3, $arg4, $arg5, $arg6, $arg7))) {
                echo "Database binding error.";
                die();
            }
            break;
        case 10:
            if (!($stmt->bind_param($type, $arg1, $arg2, $arg3, $arg4, $arg5, $arg6, $arg7, $arg8))) {
                echo "Database binding error.";
                die();
            }
            break;
        default:
            echo "Invalid number of arguments in bindParam. ($numargs)";
            die();
    }
}

/**
 * Executes the specified prepared statement.
 * @param mysqli_stmt $stmt The statement to execute.
 * @return bool Whether the execution was successful.
 */
function executeStatement($stmt) {
    return $stmt->execute();
}

/**
 * Gets the first row of results from a mysqli statement.
 * @param mysqli_stmt $stmt The executed statement to get the result from.
 * @return array|null The row of results (if any).
 */
function getSingleResult($stmt) {
    $result = $stmt->get_result();
    return mysqli_fetch_assoc($result);
}

/**
 * Gets an array of baby profiles associated with the account.
 * @param mysqli $db         The mysqli database interface.
 * @param String $user_email The logged in user's email address.
 * @return array An array of profile {id, name, dob}
 */
function getProfiles($db, $user_email) {
  $stmt = prepareQuery($db, 'SELECT p.id, p.name, p.gender, p.dob ' .
    'FROM BabyUsers as u ' .
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

