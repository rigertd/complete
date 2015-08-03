<?php
/**
 * Stores common session code.
 * User: David Rigert
 * Date: 8/1/2015
 * Time: 12:19 PM
 */

session_save_path('../../../temp/cs340');
session_start();

/* Check if user is logged in */
if (session_status() != PHP_SESSION_ACTIVE || !isset($_SESSION['username'])){
    /* for building URL to current path */
    $host = $_SERVER['HTTP_HOST'];
    $url = rtrim(dirname($_SERVER['PHP_SELF']), "\\/");
    /* no username or session--redirect to login.php */
    header("Location: https://{$host}{$url}/login.php");
    die();
}

/* get user info */
$username = $_SESSION['username'];
$user_id = $_SESSION['user_id'];
$first = $_SESSION['first'];
$issue_sort_col = $_SESSION['issue_sort_col'];
$issue_sort_dir = $_SESSION['issue_sort_dir'];
$proj_sort_col = $_SESSION['proj_sort_col'];
$proj_sort_dir = $_SESSION['proj_sort_dir'];
$ui_lang_id = $_SESSION['ui_lang_id'];
$ui_lang_name = $_SESSION['ui_lang_name'];
$alert = '';

/* store JS alert over postback */
if (isset($_SESSION['alert'])) {
    $alert = $_SESSION['alert'];
    unset($_SESSION['alert']);
}
