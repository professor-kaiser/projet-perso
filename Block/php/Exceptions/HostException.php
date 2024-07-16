<?php

class HostException extends Exception {
	/**
	*	Types of Exception values
	* 
	* 	@var <const>
	*/
	const __HOST_UNREACHABLE__ = 0;
	const __HOST_BLOCKED__ = 1;

	/**
	*	Type of Exception
	* 
	*	@var <int>
	*/
	private $type;

	/**
	*	Hostname producing Exception
	* 
	* 	@var <string>
	*/
	private $hostname;

	public function __construct(int $type, string $hostname) {
		$this->type = $type;
		$this->hostname = $hostname;
		$this->setMessage();
	}

	/**
	*	Exception message according to type of Exception
	* 
	* 	@return <void>
	*/
	public function setMessage() : void {
		switch($this->type) {
			case HostException::__HOST_UNREACHABLE__:
				$this->message = "Host [ $this->hostname ] unreachable";
			break;
			case HostException::__HOST_BLOCKED__:
				$this->message = "Blocking [ $this->hostname ] may be blocked application's host";
			break;
			default: break;
		}
	}
}