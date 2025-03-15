from .parser import Parser, ParserCode
from pyparsing import *

class CreateDBParser(Parser):
	def load(self):
		identifier = Word(alphas)

		query = (
			Suppress('CREATE') +
			Suppress('DATABASE') +
			identifier.setResultsName('database')
		).setParseAction(lambda t: {'type': ParserCode.CREATE_DATABASE.value, **t.asDict()})

		return query