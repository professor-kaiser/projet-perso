from data.type import EpycType
from parser import parser, ParserCode
from pathlib import Path
import json
import os

DB_DIR = 'database'
DB_NAME = 'epyc'

class QueryProcessor:
	@staticmethod
	def process(data: list):
		table = f"{DB_DIR}/{DB_NAME}/{data['table']}.epyc"
		table_record = Path(f"{DB_DIR}/{DB_NAME}/{data['table']}.record")

		match data['type']:
			#------------------------------------------------------------------------#
			# CREATE TABLE 															 #
			#------------------------------------------------------------------------#
			case ParserCode.CREATE_TABLE.value:
				if not Path(table).exists():
					Path(table).touch()
					with open(table, 'w') as file:
						json.dump(data, file)
				return json.dumps({'status': 201, 'operation': 'CREATE TABLE'})
			#------------------------------------------------------------------------#

			#------------------------------------------------------------------------#
			# INSERT INTO 															 #
			#------------------------------------------------------------------------#
			case ParserCode.INSERT.value:
				record = []
				prev_record = []

				if table_record.is_file():
					if Path(table_record).stat().st_size > 0:
						with open(table_record, 'r') as file:
							prev_record = json.load(file)
					else:
						table_record.touch()

				with open(table, 'r') as file:
					struct = json.load(file)
					column_types = {col["name"]: EpycType[col["datatype"]] for col in struct["columns"]}

					for row in data['values']:
						"""---------------------------------------------------------------------------------------------------------------
							Verify record unicity
						------------------------------------------------------------------------------------------------------------------"""
						columns_set  = {col["name"] for col in struct["columns"]}
						row_matched  = dict(zip(data['columns'], row))
						row_complete = {key: row_matched.get(key, None) for key in columns_set}

						if row_complete in prev_record:
							return json.dumps({'rows': row, 'cols': None, 'status': {'code': 'Error', 'message': f"{row} already exists"}})
						#------------------------------------------------------------------------------------------------------------------#

						"""---------------------------------------------------------------------------------------------------------------
							Check if the type matches
						------------------------------------------------------------------------------------------------------------------"""
						if not all(column_types[col].check(value) for col, value in zip(data['columns'], row)):
							return json.dumps({'status': 'error'})
						#------------------------------------------------------------------------------------------------------------------#

					record = [
						{col["name"]: values[i] if i < len(values) else None 
          				for i, col in enumerate(struct["columns"])}
          				for values in data["values"]
          			]

				record.extend(prev_record)
				with open(table_record, 'w') as file:
					json.dump(record, file)

				return json.dumps(record)
			#------------------------------------------------------------------------#

			#------------------------------------------------------------------------#
			# SELECT																 #
			#------------------------------------------------------------------------#
			case ParserCode.SELECT.value:
				rows = []
				result = []

				with open(table_record, 'r') as file:
					rows = json.load(file)

				# Select only columns specified in SELECT QUERY
				rows = [{
					k: v for k, v in row.items() 
					if k in list(data['columns'])
				} for row in rows]

				# If SELECT Query has condition
				if 'condition' not in data:
					result = rows
				# Else
				else:
					# Match condition for all rows
					for row in rows:
						if QueryProcessor.match(row, data['condition']):
							result.append(row)

				# If Query contains ORDER BY
				if 'order' in data:
					result = sorted(result, key = lambda t: tuple (
						#-------------------------------------------------------------------------------------------#
						# ASCENDING                                                                                 #
						#-------------------------------------------------------------------------------------------#
							t[column] if direction == 'ASC'                                                         #
						#-------------------------------------------------------------------------------------------#

						#-------------------------------------------------------------------------------------------#
						# DESCENDING																			    #
						#-------------------------------------------------------------------------------------------#
						else ( 																					    #
							#------------------------------------------------------------------------------------#  #
							# Type: NUMERIC                                                                      #  #
							#------------------------------------------------------------------------------------#  #
							-t[column] if isinstance(t[column], (int, float)) and t[column] is not None          #  #
							#------------------------------------------------------------------------------------#  #
						else																				        #
							#------------------------------------------------------------------------------------#  #
							# Type: DATE TIME 															            #
							#------------------------------------------------------------------------------------#  #
							-t[colonne].timestamp() if isinstance(t[colonne], datetime) and t[column] is not None#  #
							#------------------------------------------------------------------------------------#  #
						else 																					    #
							#------------------------------------------------------------------------------------#  #
							# Type CHARACTER                                                                     #  #
							#------------------------------------------------------------------------------------#  #
							f"~{t[column]}" if t[column] is not None else None                                   #  #
							#------------------------------------------------------------------------------------#  #
						)																							#
						for column, direction in data['order'].items()												#
						#-------------------------------------------------------------------------------------------#
					))

				return json.dumps({'rows': result, 'columns': data['columns']})
			#------------------------------------------------------------------------#

	@staticmethod
	def operate(row, condition):
		column   = condition['column']
		operator = condition['operator']
		value    = condition['value']

		match operator:
			case '>':
				return row[column] > value

			case '>=':
				return row[column] >= value

			case '!=':
				return row[column] != value

			case '<':
				return row[column] < value

			case '<=':
				return row[column] <= value

			case '=':
				return row[column] == value

			case 'IN':
				result = json.loads(QueryProcessor.process(value[0]))
				return {column: row[column]} in result

	@staticmethod
	def match(row, conditions: list):
		result = True
		operator = None

		if not isinstance(conditions, list):
			conditions = [conditions]

		for condition in conditions:
			if isinstance(condition, str):
				operator = condition

			elif isinstance(condition, dict):
				if operator != None:
					result = QueryProcessor.validate (
						result,
						operator,
						QueryProcessor.operate(row, condition)
					)
				else:
					result = QueryProcessor.operate(row, condition)

			elif isinstance(condition, list):
				result = QueryProcessor.match(row, condition)

		return result

	@staticmethod
	def validate(cond1, operator, cond2):
		match operator:
			case 'AND':
				return cond1 and cond2

			case 'OR':
				return cond1 or cond2