from system import Connection

connection = Connection('epyc://127.0.0.1:8910/epyc')
statement = connection.create_statement()
resultset = None

statement.query ( """
	SELECT id, name, numero 
	FROM client 
	WHERE id < 4 OR name = 'Titi'
	ORDER BY name ASC, id ASC
""" )

resultset = statement.execute()

while resultset.next():
	print(f"{resultset.get('id')} - {resultset.get('name')} - {resultset.get('numero')}")

connection.close()

"""
Tested query:

SELECT:
	SELECT id, name FROM client
	SELECT id, name FROM client WHERE id != 3
	SELECT id, name FROM client WHERE ((name = 'Jean' AND id = 1) OR id = 2) OR name = 'Titi'

	---------------------------------
	SELECT id, name, numero 
	FROM client 
	WHERE id < 4 OR name = 'Titi'
	ORDER BY (name ASC, id DESC)
	---------------------------------
"""