class ResultSet:
	def __init__(self, rows, columns: list):
		self._rows = rows
		self._cursor = -1
		self._columns = list(columns)

		#self._rows = sorted(self._rows, key = lambda t: tuple(t[colonne] for colonne in order))
		print(self._rows)

	def next(self):
		if self._cursor + 1 < len(self._rows):
			self._cursor += 1;
			return True
		return False

	def get(self, column):
		return self._rows[self._cursor][self._columns[self._columns.index(column)]]