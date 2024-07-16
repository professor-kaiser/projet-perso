<?php
require_once 'php/Firewall.php';

if(!isset($_POST['password'])) {
	$_POST['password'] = null;
}

$url      = $_POST['url'];
$password = $_POST['password'];
$fw = new Firewall();

try {
	$fw->allow($url, $password);
	header('Location: index.php?service=stop');
}
catch(Exception $e) {
	header("Location: index.php?err=1&service=stop");
}

