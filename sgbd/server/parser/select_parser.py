from .parser import Parser, ParserCode
from pyparsing import *

class SelectParser(Parser):
	def load(self):
		query      = Forward()
		subquery   = Group(('(' + query + ')').setParseAction(lambda t: t[1:-1])).setResultsName('value')
		identifier = Word(alphas)

		"""---------------------------------------------------------------------
			SQL keywords and identifiers                                                                 
		---------------------------------------------------------------------"""
		SELECT       = Suppress('SELECT')
		columns      = delimitedList(identifier).setResultsName('columns')
		FROM         = Suppress('FROM')
		table        = identifier.setResultsName("table")
		WHERE        = Suppress('WHERE')
		conditions   = SelectParser.__general_condition(subquery)
		ORDER_BY     = Suppress('ORDER BY')
		column_order = delimitedList(Group(identifier + identifier)) 
		#----------------------------------------------------------------------#

		"""--------------------------------------------------------------------------
			SQL Syntax                                                                 
		--------------------------------------------------------------------------"""
		query <<= (
			SELECT + columns + FROM + table + Optional (
			WHERE + conditions + Optional (
			(ORDER_BY + column_order)
				.setParseAction(lambda t: {order[0] : order[1] for order in t})
				.setResultsName('order')
			)
			)
		).setParseAction(lambda t: {'type': ParserCode.SELECT.value, **t.asDict()})
		#---------------------------------------------------------------------------#

		"""
		query <<= (
            SQL('SELECT') +
			delimitedList(identifier).setResultsName("columns") +
			SQL('FROM') +
			identifier.setResultsName("table") + 
			Optional (
				SQL('WHERE') + 
				SelectParser.__general_condition(subquery) +
				Optional((
					Suppress('ORDER BY') + delimitedList(Group(identifier + identifier)) 
				).setParseAction(lambda t: {order[0] : order[1] for order in t}).setResultsName('order'))
				, default = None
			)
        ).setParseAction(lambda t: {'type': ParserCode.SELECT.value, **t.asDict()})
        """
		
		return query

	"""
	@staticmethod
	def __sql_keywords():
		sql_keywords = {
			'SELECT': Suppress('SELECT'),
			'FROM'  : Suppress('FROM'),
			'WHERE' : Suppress('WHERE'),
			'AND'   : Literal('AND'),
			'OR'    : Literal('OR'),
		}

		return sql_keywords
	"""

	@staticmethod
	def __condition_value():
		# Type of value used by condition
		number = Word(nums).setParseAction(lambda t: int(t[0]))
		string = quotedString.setParseAction(lambda t: t[0][1:-1])
		value  = number | string

		return value

	@staticmethod
	def __unit_condition(subquery = None):
		identifier = Word(alphas)
		operator   = oneOf('> >= = != < <= IN')
		value      = SelectParser.__condition_value()

		unit_condition = Group (
			identifier.setResultsName("column") +
			operator.setResultsName("operator") +
			(value.setResultsName("value") | subquery)
		)

		return unit_condition

	@staticmethod
	def __general_condition(subquery = None):
		SQL = lambda kw: SelectParser.__sql_keywords()[kw]

		general_condition = operatorPrecedence ( SelectParser.__unit_condition(subquery), [
			(SQL('AND'), 2, opAssoc.LEFT),
			(SQL('OR'), 2, opAssoc.LEFT)
		]).setResultsName("condition")

		return general_condition