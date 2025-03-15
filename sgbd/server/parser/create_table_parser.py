from .parser import Parser, ParserCode
from pyparsing import *

class CreateTableParser(Parser):
	def load(self):
		identifier = Word(alphas)

		column = Group(
			identifier.setResultsName('name') +
			identifier.setResultsName('datatype')
		)

		query = (
			Suppress('CREATE') + Suppress('TABLE') + identifier.setResultsName('table') + 
			Suppress('(') +
				delimitedList(column).setResultsName('columns') +
			Suppress(')')
		).setParseAction(lambda t: {'type': ParserCode.CREATE_TABLE.value, **t.asDict()})

		return query