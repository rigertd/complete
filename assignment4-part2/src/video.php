<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

/* Establish database connection */
$mysqli = new mysqli("oniddb.cws.oregonstate.edu", "rigertd-db", "7UIl485kmwS6rujQ", "rigertd-db");
if ($mysqli->connect_errno) {
    echo "Database connection error (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    die();
}

/* for storing any error messages when attempting to add a title */
$err_msg = "";

/* query database for title information */
$inventory = $mysqli->query("SELECT id, name, category, length, rented FROM Inventory ORDER BY name ASC");
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
        <label>Runtime <input type="number" name="length"></label>
<?php if (!empty($err_msg)): ?>
        <p class="error"><?php echo htmlspecialchars($err_msg); ?>
<?php endif ?>
        <button type="submit" name="action" value="add">Add</button>
      </form>
    </section>
    <section>
      <h2>Current Inventory</h2>
      <form method="POST" action="video.php">
        <label for="category_list">Filter by Category: </label>
        <select id="category_list">
          <option value="All Movies">All Movies</option>
<?php foreach ($categories as $category): ?>
          <option value="<?php echo htmlspecialchars($category); ?>"><?php echo htmlspecialchars($category); ?></option>
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
              <button type="submit" name="action" value="check_out">Check Out</button>
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
