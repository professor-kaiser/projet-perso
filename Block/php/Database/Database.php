<?php

class Database {
	private $PDO;
	const __TABLE__ = 'Blocked';

	public function __construct() {
		require_once 'db_info.php';

		$dsn = "$db:host=$host;dbname=$db_name";
		$this->PDO = new PDO($dsn, $db_user, $db_password);
	}

	public function insert(string $url, string $password = null) {
		$req = $this->PDO->prepare('INSERT INTO '.Database::__TABLE__.'(url, action_date, password) 
																	VALUES (:url, NOW(), :password)');
		$req->bindValue(':url', $url);
		$req->bindValue(':password', $password);
		$req->execute();
	}

	public function delete(string $url) {
		$req = $this->PDO->prepare('DELETE FROM '.Database::__TABLE__.' WHERE url=:url');
		$req->bindValue(':url', $url);
		$req->execute();
	}

	public function exists(string $url) : bool {
		$req = $this->PDO->prepare('SELECT url FROM '.Database::__TABLE__.' WHERE url=:url');
		$req->bindValue(':url', $url);
		$req->execute();

		$result = $req->fetchAll(PDO::FETCH_ASSOC);
		return (empty($result) ? false:true);
	}

	public function findAll() {
		$req = $this->PDO->query('SELECT * FROM '.Database::__TABLE__.' ORDER BY action_date');
		$result = $req->fetchAll(PDO::FETCH_ASSOC);

		return $result;
	}

	public function find(string $url) {
		$req = $this->PDO->prepare('SELECT * FROM '.Database::__TABLE__.' WHERE url LIKE :url ORDER BY action_date');
		$req->bindValue(':url', '%'.$url.'%');
		$req->execute();

		$result = $req->fetchAll(PDO::FETCH_ASSOC);
		return $result;
	}

	public function findJoinable() {
		$result = [];
		$req = $this->PDO->query('SELECT * FROM '.Database::__TABLE__.' ORDER BY action_date');
		foreach($req->fetchAll(PDO::FETCH_ASSOC) as $data) {
			if(checkdnsrr($data['url'], 'A') || checkdnsrr($data['url'], 'AAAA')) {
				$result[] = $data;
			}
		}

		return $result;
	}

	public function is_locked(string $url) : bool {
		$req = $this->PDO->prepare('SELECT password FROM '.Database::__TABLE__.' WHERE url=:url');
		$req->bindValue(':url', $url);
		$req->execute();

		$result = $req->fetch(PDO::FETCH_ASSOC);

		return (empty($result['password']) ? false:true);
	}

	public function password_for(string $url) {
		$req = $this->PDO->prepare('SELECT password FROM '.Database::__TABLE__.' WHERE url=:url');
		$req->bindValue(':url', $url);
		$req->execute();

		$result = $req->fetch(PDO::FETCH_ASSOC);

		return $result['password'];
	}
}