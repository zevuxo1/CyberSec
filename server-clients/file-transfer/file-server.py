#!/bin/python3

import socket
import argparse

parser = argparse.ArgumentParser(description="Simple File Transfer Server")
parser.add_argument('-p', '--port', type=int, required=True, help="port to Listen on")
parser.add_argument('-i', '--ip', type=str, required=True, help="IP to listen")


args = parser.parse_args()


host = args.ip
tport = args.port


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind((host,tport))

server.listen()
print(f"[*] Listening On {host}:{tport}")

while True:
	con, addr = server.accept()

	print(f"[*] Connection From {addr[0]}:{addr[0]}")
	print("*#" * 25)

	## receive the filename
	filename = con.recv(1024).decode()
	print("[*] File Is Being Sent")
	print(f"[*] Filename Is {filename}")
	file = open(filename, 'w')
	con.send("[*] File Name Recveived".encode('utf-8'))

	## receive the file contents
	data = con.recv(1024).decode()
	print("[*] Receving File Contents")
	file.write(data)
	con.send("[*] Received Contents".encode('utf-8'))

	file.close()

	con.close()
	break

server.close()



