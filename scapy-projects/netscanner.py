#!/bin/python

from scapy.all import Ether, ARP, srp
from termcolor import colored
from termcolor import cprint

def menu():
	print("#" * 36)
	cprint('      IP', 'green', attrs=['bold'], end="")
	print(" " * 8, "|", end="")
	print(" " * 8, end="")
	cprint(' MAC', 'green', attrs=['bold'])
	print("#" * 36)

clients = []

target_ip = "192.168.1.1/24"

arp = ARP(pdst=target_ip)

ether = Ether(dst="ff:ff:ff:ff:ff:ff")

packet = ether/arp

res = srp(packet, timeout=1, verbose=1)[0]

for sent, received in res:
	clients.append({"IP": received.psrc, "mac": received.hwsrc})
	

menu()
for client in clients:
	print("{:16}   {}".format(client['IP'], client['mac']))