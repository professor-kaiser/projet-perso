from .parser import Parser, ParserCode
from pyparsing import *

class InsertParser(Parser):
	def load(self):
		identifier = Word(alphas)
		value      = InsertParser.__condition_value()

		columns = Group (
			('(' + delimitedList (identifier) + ')').setParseAction(lambda t: t[1:-1])
		).setResultsName('columns')

		values = Group (
			('(' + delimitedList(value) + ')').setParseAction(lambda t: t[1:-1])
		)

		query = (
			Suppress('INSERT') + Suppress('INTO')  + identifier.setResultsName('table') +
			columns + Suppress('VALUES') + delimitedList(values).setResultsName('values')
		).setParseAction(lambda t: {'type': ParserCode.INSERT.value, **t.asDict()})

		return query

	@staticmethod
	def __condition_value():
		# Type of value used by condition
		number = Word(nums).setParseAction(lambda t: int(t[0]))
		string = quotedString.setParseAction(lambda t: t[0][1:-1])
		value  = number | string

		return value