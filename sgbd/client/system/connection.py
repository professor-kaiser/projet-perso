from pyparsing import *
import socket

class Connection:
	def __init__(self, url: str):
		parsed_url = Connection.__url_parser().parseString(url)
		self._host = parsed_url.host
		self._port = parsed_url.port
		self._database = parsed_url.database
		self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self._socket.connect((self._host, self._port))
		self._is_connected = True

	def create_statement(self):
		from .statement import Statement
		return Statement(self)

	def close(self):
		if self._is_connected:
			self._socket.close()
			self._is_connected = False

	@staticmethod
	def __url_parser():
		identifier = Word(alphas) | Word(nums).setParseAction(lambda t: int(t[0]))
		bit = Word(nums, min = 1, max = 3)
		ipv4 = Combine(bit + '.' + bit + '.' + bit + '.' + bit)
		host = ipv4 | Word(alphas)

		url = (
			Suppress('epyc://') + host('host') + Suppress(':') +
			identifier.setResultsName('port') + Suppress('/') + identifier.setResultsName('database')
		)

		return url