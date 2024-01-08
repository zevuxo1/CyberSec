#!/bin/python3

import socket
import argparse
import os

parser = argparse.ArgumentParser(description="Simple File Transfer Server")
parser.add_argument('-p', '--port', type=int, required=True, help="port to Listen on")
parser.add_argument('-i', '--ip', type=str, required=True, help="IP to connect To")
parser.add_argument('-f', '--files', type=str, required=True, help="files To Send")

args = parser.parse_args()


host = args.ip
tport = args.port
buffer = 1024
file = args.files

filename = os.path.basename(file)

fi = open(file, 'r')
data = fi.read()

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((host,tport))

client.send(filename.encode('utf-8'))
msg = client.recv(1024).decode()
print(msg)

client.send(data.encode('utf-8'))
msg = client.recv(1024).decode()
print(msg)

fi.close()
client.close()