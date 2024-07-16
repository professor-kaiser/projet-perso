<?php
session_start();

require_once 'php/Firewall.php';

$fw = new Firewall();

try {
	if(isset($_POST['status']) && !empty($_POST['status'])) {
		$fw->start();
		$_SESSION['status'] = $fw->getStatus();
		header('Location: index.php');
	}
	else {
		$fw->stop();
		$_SESSION['status'] = $fw->getStatus();
		header('Location: index.php?service=stop');
	}
}
catch(HostException $he) {
	echo $he->getMessage();
}
catch(Exception $e) {
	echo $e->getMessage();
}
