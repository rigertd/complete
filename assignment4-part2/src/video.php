<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

/* Prepares the specified query in a prepared statement */
function prepareQuery($db, $query) {
  if (!($stmt = $db->prepare($query))) {
    echo "Database query error (" . $db->errno . ") " . $db->error;
    die();
  }
  return $stmt;
}

/* Binds the specified parameter to the specified prepared statement */
function bindParam($stmt, $type, $val) {
    if (!$stmt->bind_param($type, $val)) {
        echo "Database binding error (" . $stmt->errno . ") " . $stmt->error;
        die();
    }
}

/* Executes the specified prepared statement */
function executeStatement($stmt) {
    if (!$stmt->execute()) {
        echo "Error executing operation (". $stmt->errno . ") " . $stmt->error;
        die();
    }
}

function addMovie() {
    global $err_msg;
    global $mysqli;

    $name = $_POST['name'];
    $category = isset($_POST['category']) ? $_POST['category'] : null;
    $length = isset($_POST['length']) ? $_POST['length'] : null;
    /* validate input (just in case) */
    if (empty($name))
        $err_msg .= "You must enter the title of the movie. ";
    else if (!empty($length) && !ctype_digit($length))
        $err_msg .= "The runtime must be a positive number.";
    else {
        $add_query = "INSERT INTO Inventory (name, category, length) VALUES(?, ?, ?)";
        /* prepare statement, bind parameters, and execute */
        $add_stmt = prepareQuery($mysqli, $add_query);
        $add_stmt->bind_param("ssi", $name, $category, $length);
        executeStatement($add_stmt);
    }
}

/* Establish database connection */
$mysqli = new mysqli("oniddb.cws.oregonstate.edu", "rigertd-db", "7UIl485kmwS6rujQ", "rigertd-db");
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/* for storing any error messages when attempting to add a title */
$err_msg = "";

/* for storing the category to filter on--defaults to All Movies */
$filter = "All Movies";

/* query database for title information */
$inv_query = "SELECT id, name, category, length, rented FROM Inventory ORDER BY name ASC";

/* check if request is a postback and perform the appropriate action */
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['action'])) {
  switch ($_POST['action']) {
    case "add":
      addMovie();
      break;
    case "filter":
      $filter = $_POST['category_list'];
      if ($filter != "All Movies")
        $inv_query = "SELECT id, name, category, length, rented FROM Inventory WHERE category = ? ORDER BY name ASC";
      break;
    case "delete":
      $del_id = (int)$_POST['id'];
      $del_query = "DELETE FROM Inventory WHERE id = ?";
      $del_stmt = prepareQuery($mysqli, $del_query);
      bindParam($del_stmt, "i", $del_id);
      executeStatement($del_stmt);
      break;
    case "check_out":
      $co_id = (int)$_POST['id'];
      $co_query = "UPDATE Inventory SET rented = IF(rented, 0, 1) WHERE id = ?";
      $co_stmt = prepareQuery($mysqli, $co_query);
      bindParam($co_stmt, "i", $co_id);
      executeStatement($co_stmt);
      break;
    case "delete_all":
      if (!$mysqli->query("DELETE FROM Inventory")) {
        echo "Deletion failed (" . $mysqli->errno . ") " . $mysqli->error;
        die();
      }
      break;
    default:
      $err_msg .= "Invalid action in POST request.";
  }
}

/* Prepare, bind, and execute inventory list query */
$inv_stmt = prepareQuery($mysqli, $inv_query);
if ($filter != "All Movies")
  bindParam($inv_stmt, "s", $filter);
executeStatement($inv_stmt);

/* get results */
$inventory = $inv_stmt->get_result();

/* query database for category information */
$cat_result = $mysqli->query("SELECT DISTINCT category FROM Inventory WHERE category IS NOT NULL ORDER BY category ASC");

/* close the connection after results are retrieved */
$mysqli->close();

/* build an array of categories */
$categories = array();
while ($row = $cat_result->fetch_assoc()) {
    $categories[] = $row['category'];
}

?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Video Inventory System</title>
    <link rel="stylesheet" href="../style.css">
  </head>
  <body>
    <header><h1>Bob's Video Emporium</h1></header>
    <section>
      <form method="POST" action="video.php">
        <h2>Add Video to Inventory</h2>
        <label>Title <input type="text" name="name" required></label>
        <label>Category <input type="text" name="category" list="categories"></label>
        <datalist id="categories">
<?php foreach ($categories as $category): ?>
          <option value="<?php echo htmlspecialchars($category); ?>"><?php echo htmlspecialchars($category); ?></option>
<?php endforeach ?>
        </datalist>
        <label>Runtime <input type="number" name="length" min="0"></label>
        <button type="submit" name="action" value="add">Add</button>
<?php if (!empty($err_msg)): ?>
        <p class="error"><?php echo htmlspecialchars($err_msg); ?>
<?php endif ?>
      </form>
    </section>
    <section>
      <h2>Current Inventory</h2>
      <form method="POST" action="video.php">
        <label for="category_list">Filter by Category: </label>
        <select id="category_list" name="category_list">
          <option value="All Movies">All Movies</option>
<?php foreach ($categories as $category): ?>
          <option value="<?php echo htmlspecialchars($category); ?>"<?php echo ($filter == $category ? ' selected="selected"' : ''); ?>><?php echo htmlspecialchars($category); ?></option>
<?php endforeach ?>
        </select>
        <button type="submit" name="action" value="filter">Filter</button>
      </form>
      <table>
        <tr><th>ID <th>Title <th>Category <th>Runtime <th>Checked Out <th>Action </tr>
<?php while($row = $inventory->fetch_assoc()): ?>
        <tr>
          <td><?php echo $row['id']; ?>
          <td><?php echo htmlspecialchars($row['name']); ?>
          <td><?php echo htmlspecialchars($row['category']); ?>
          <td><?php echo (!empty($row['length']) ? "$row[length] minutes" : ""); ?>
          <td><?php echo ($row['rented'] ? "Yes" : "No"); ?>
          <td>
            <form method="POST" action="video.php">
              <input type="hidden" name="id" value="<?php echo $row['id']; ?>">
              <button type="submit" name="action" value="delete">Delete</button>
              <button type="submit" name="action" value="check_out"><?php echo ($row['rented'] ? "Return" : "Check Out"); ?></button>
            </form>
        </tr>
<?php endwhile ?>
      </table>
      <form method="POST" action="video.php">
        <button type="submit" name="action" value="delete_all">Delete All Videos</button>
      </form>
    </section>
  </body>
</html>
