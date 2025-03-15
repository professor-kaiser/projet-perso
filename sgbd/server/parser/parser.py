from pyparsing import *
from enum import Enum

class ParserCode(Enum):
	CREATE_DATABASE = 0
	CREATE_TABLE    = 1
	INSERT          = 2
	SELECT          = 3

class Parser:
	def __init__(self):
		self._columns = []
		self._parser = None

	def load(self):
		pass

	def query(self, sql: str):
		return self.load().parseString(sql)