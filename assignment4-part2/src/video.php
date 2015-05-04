<?php
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
      <h2>Add Video to Inventory</h2>
      <form method="POST" action="video.php">
        <label>Title <input type="text" name="name"></label>
        <label>Category <input type="text" name="category" list="categories"></label>
        <datalist id="categories">
          <option value="Action">
        </datalist>
        <label>Runtime <input type="number" name="length"></label>
        <button type="submit">Add</button>
      </form>
    </section>
    <section>
      <h2>Current Inventory</h2>
      <form method="POST" action="video.php">
        <label for="category_list">Filter by Category: </label>
        <select id="category_list">
          <option value="All Movies">All Movies</option>
        </select>
        <button type="submit">Filter</button>
      </form>
      <table>
        <tr><th>ID <th>Title <th>Category <th>Runtime <th>Checked Out <th>Action </tr>
        <tr>
          <td><?php echo $videoId ?>
          <td><?php echo $videoName ?>
          <td><?php echo $videoCategory ?>
          <td><?php echo $videoLength ?>
          <td><?php echo ($videoCheckedOut ? "Yes" : "No") ?>
          <td><!-- buttons! -->
        </tr>
      </table>
      <form method="POST" action="video.php"><button type="submit">Delete All Videos</button></form>
    </section>
  </body>
</html>