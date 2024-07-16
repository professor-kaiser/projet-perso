<?php
session_start();

require_once 'php/Firewall.php';

$_POST['password'] ?? "";

$url      = $_POST['url'];
$password = $_POST['password'];
$fw = new Firewall();

$hash = (empty($password)) ? null:password_hash($password, PASSWORD_BCRYPT);

try {
	$fw->block($url, $hash);
	header("Location: index.php");
}
catch(HostException $e) {
	$_SESSION['err_msg'] = $e->getMessage();
	header("Location: index.php?err=1");
}

