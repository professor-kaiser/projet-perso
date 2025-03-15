from abc import ABC, abstractmethod
import pickle
import json

class Statement:
	def __init__(self, connection):
		self._connection = connection
		self._query = None

	def query(self, query: str):
		self._query = query

	def execute(self):
		data = self._query.encode('utf-8')
		self._connection._socket.sendall(data)
		response = self._connection._socket.recv(1024).decode('utf-8')
		response = json.loads(response)

		from .resultset import ResultSet
		return ResultSet(response['rows'], response['columns'])
