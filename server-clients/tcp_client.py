#!/bin/python3

import argparse
import socket

parser = argparse.ArgumentParser(description="TCP CLIENT")
parser.add_argument("-p", "--port", type=int, required=True, help="Target Port To Connect Too")
parser.add_argument("-t", "--target", type=str, required=True, help="Target IP to Connect Too")
parser.add_argument("-m", "--message", type=str, required=True, help="Message To send")

args = parser.parse_args()

host = args.target
tport = args.port
msg = args.message

c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

c.connect((host, tport))

c.send(msg.encode('utf-8'))

req = c.recv(4096)

print(req.decode())
