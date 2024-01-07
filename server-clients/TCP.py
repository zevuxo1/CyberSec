#!/bin/python3

import socket
import argparse
import threading

parser = argparse.ArgumentParser(description="TCP CLIENT")
parser.add_argument("-p", '--port', type=int, required=True, help="Target Port")

args= parser.parse_args()


tport = args.port
host = '0.0.0.0'

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, tport))

s.listen(5)

print(f"[*] Listening On {host}:{tport}")


def handle_client(client_socket):
	try:
		with client_socket as sock:
			req = sock.recv(1024)
			print(f"[*] Message From {addr[1]}: {req.decode()}")
			sock.send("ACK".encode('utf-8'))
			sock.close()

	except KeyboardInterrupt:
		print("Quitting")

while True:
	client, addr = s.accept()
	print(f"[*] Got A Connection From {addr[0]}:{addr[1]}")
	client_hand = threading.Thread(target=handle_client, args=(client, ))
	client_hand.start()
	s.close()


