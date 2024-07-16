<?php

const DNS_UNREACHABLE = 2;

set_error_handler(function($errno, $errstr, $errfile, $errline) {
	if($errno === DNS_UNREACHABLE) {
		throw new Exception("Error: $errstr on file: $errfile [line $errline]");
	}
});