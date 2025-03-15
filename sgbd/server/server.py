import socket
from parser import parser
from data.proc import QueryProcessor

host, port = ('', 8910)

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.bind((host, port))

while True:
	socket.listen(5)
	conn, address = socket.accept()
	print(f'listen: {address}')

	data = conn.recv(1024)
	data = data.decode('utf-8')

	print(parser.parseString(data)[0])

	print(parser.parseString(data)[0])

	response = QueryProcessor.process(parser.parseString(data)[0])
	conn.send(response.encode('utf-8'))

conn.close()
socket.close()