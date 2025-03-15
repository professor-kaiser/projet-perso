from enum import Enum
from datetime import date, time, datetime
import decimal

class EpycType(Enum):
	"""---------------------------
	Type NUMERIC
	---------------------------"""
	INTEGER = int
	FLOAT   = float
	#----------------------------

	"""---------------------------
	Type CHARACTER
	---------------------------"""
	CHARACTER = str
	#-----------------------------

	"""---------------------------
	Type DATE and TIME
	---------------------------"""
	DATE      = date
	TIME      = time
	DATETIME  = datetime
	TIMESTAMP = datetime
	#-----------------------------

	"""---------------------------
	Type BOOLEAN
	---------------------------"""
	BOOLEAN = bool
	#-----------------------------

	def check(self, value):
		return isinstance(value, self.value)