<?php
error_reporting(E_ALL);
ini_set('display_errors', 'On');

session_start();

/* Check if user is logged in */
if (session_status() != PHP_SESSION_ACTIVE || !isset($_SESSION['email'])){
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no username or session--redirect to login.php */
    header("Location: http://{$host}{$url}/login.php");
    die();
}

/* get user info */
$first = $_SESSION['first'];
$last = $_SESSION['last'];
$email = $_SESSION['email'];

?>
<meta charset="utf-8">
<meta http-equiv="x-ua-compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Baby Growth Tracker - Dashboard</title>
<link href="../css/bootstrap.min.css" rel="stylesheet">
<!--[if lt IE 9]>
<script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
<script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
<![endif]-->
<link href="../css/style.css" rel="stylesheet">
<script src="../js/login.js"></script>
</head>
<body>
  <div class="container">
    <header class="page-header clearfix">
      <h2 class="col-sm-9">Baby Growth Tracker</h2>
      <div class="col-sm-3"><a href="login.php?action=logout" class="btn btn-default" role="button">Log Out</a></div>
    </header>
    <p>Hello <?php echo $first ?>, welcome to the dashboard.</p>
    <footer>&copy; David Rigert</footer>
  </div>
</body>
</html>