<?php
require_once 'Database/Database.php';
require_once 'php/Exceptions/HostException.php';

class Firewall {
	/**
	*	Utils constantes for iptables
	*
	*	@var <const>
	*/
	const __INPUT_RULES__  = 'input.sh';
	const __OUTPUT_RULES__ = 'output.sh';
	const __FLUSH_RULES__  = 'flush.sh';
	const __BASE_RULE__ = 'sudo iptables -t filter -p tcp -m multiport --dports http,https';
	const __INPUT__  = ' -A INPUT ';
	const __OUTPUT__ = ' -A OUTPUT ';

	/**
	*	value of status of service	
	* 
	* 	@var <const int>
	*/
	const __SERVICE_START__ = 1;
	const __SERVICE_STOP__  = 0;

	/**
	*	status of service	
	* 
	* 	@var <int> [ status of service ]
	*/
	private $status;

	/**
	*	Object Database	
	* 
	*	@var <class::Database> [ database ]
	*/
	private $db;

	/**
	*	Application's hostname
	* 
	*	@var <string> [ hostname ]
	*/
	private $hostname;

	/**
	*	All IP addresses in relation with hostname
	* 	
	*	@var <array> [ IP ]
	*/
	private $ip;

	public function __construct() {
		$this->db = new Database();
		$this->hostname = getallheaders()['Host'];
		$this->ip = gethostbynamel($this->hostname);
	}

	/**
	*	Block url INPUT & OUTPUT stream
	* 
	* 	@return <void>	
	*/
	public function block(string $url, string $password = null) {
		if(!$this->db->exists($url)) {
			if(checkdnsrr($url, 'A') || checkdnsrr($url, 'AAAA')) {
				if($this->has_same_host($url)) {
					throw new HostException(HostException::__HOST_BLOCKED__, $url);
				}
				$this->db->insert($url, $password);
				$this->rewrite_rules();
			}
			else {
				throw new HostException(HostException::__HOST_UNREACHABLE__, $url);
			}
		}
		else {
			throw new Exception('URL already blocked');
		}
	}

	/**
	*	Allow url from the list of url blocked
	* 
	* 	@return <void>
	*/
	public function allow(string $url, string $password = null) {
		if($this->db->exists($url)) {
			if($this->db->is_locked($url)){
				$hash = $this->db->password_for($url);
				if(password_verify($password, $hash)){
					$this->db->delete($url);
					$this->rewrite_rules();
				}
				else {
					throw new Exception('Wrong password');
				}
			}
			else{
				$this->db->delete($url);
				$this->rewrite_rules();
			}
		}
		else {
			throw new Exception('URL not found');
		}
	}

	/**
	*	Rewrite iptables rules
	* 
	*	@return <void>
	*/
	private function rewrite_rules() {
		$input  = fopen(Firewall::__INPUT_RULES__,  'ab+');
		$output = fopen(Firewall::__OUTPUT_RULES__, 'ab+');

		exec('echo "#!/bin/bash" > '.Firewall::__INPUT_RULES__);
		exec('echo "#!/bin/bash" > '.Firewall::__OUTPUT_RULES__);

		foreach ($this->db->findJoinable() as $data) {
			$block  = ' -j DROP ';
			$source      = ' -s '.$data['url'];
			$destination = ' -d '.$data['url'];

			fwrite($input,  Firewall::__BASE_RULE__.Firewall::__INPUT__  . $block.$source."\n");
			fwrite($output, Firewall::__BASE_RULE__.Firewall::__OUTPUT__ . $block.$destination."\n");
		}

		fclose($input);
		fclose($output);
	}

	/**
	*	Start service
	* 
	*	@return <void>
	*/
	public function start() : void {
		$this->rewrite_rules();
		$this->verify_host();
		exec('sudo iptables -F');
		exec('./' . Firewall::__INPUT_RULES__);
		exec('./' . Firewall::__OUTPUT_RULES__);

		$this->status = Firewall::__SERVICE_START__;
	}

	/**
	*	Stop service
	* 
	*	@return <void>
	*/
	public function stop() : void {
		exec('sudo iptables -F');
		exec('./' . Firewall::__FLUSH_RULES__);

		$this->status = Firewall::__SERVICE_STOP__;
	}

	/**
	*	Get status of service
	* 
	*	@return <int> [ status of service ]
	*/
	public function getStatus() : int {
		return $this->status;
	}

	public function has_same_host(string $hostname) {
		$validation = false;

		foreach(gethostbynamel($hostname) as $ip) {
			if(in_array($ip, $this->ip)) {
				$validation = true;
			}
		}

		return $validation;
	}

	/**
	*
	*	@return <void>
	*/
	private function verify_host() {
		foreach ($this->db->findJoinable() as $data) {
			if($this->has_same_host($data['url'])) {
				throw new HostException(HostException::__HOST_BLOCKED__, $data['url']);
			}
		}
	}
}