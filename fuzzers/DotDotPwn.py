#!/bin/python3

import requests
import argparse
import pyfiglet
from termcolor import colored
from termcolor import cprint
import os
import time
import socket



logo = """###################################################################################################
#                                                                                                 #
#                                    ('-. .-.                                .-') _   _ .-') _    #
#                                   ( OO )  /                               ( OO ) ) ( (  OO) )   #
#   ,--.         ,------.   ,-.-')  ,--. ,--.  .-'),-----.  ,--. ,--.   ,--./ ,--,'   \     .'_   #
#   |  |.-')  ('-| _.---'   |  |OO) |  | |  | ( OO'  .-.  ' |  | |  |   |   \ |  |\   ,`'--..._)  #
#   |  | OO ) (OO|(_\       |  |  \ |   .|  | /   |  | |  | |  | | .-') |    \|  | )  |  |  \  '  #
#   |  |`-' | /  |  '--.    |  |(_/ |       | \_) |  |\|  | |  |_|( OO )|  .     |/   |  |   ' |  #
#  (|  '---.' \_)|  .--'   ,|  |_.' |  .-.  |   \ |  | |  | |  | | `-' /|  |\    |    |  |   / :  #
#   |      |    \|  |_)   (_|  |    |  | |  |    `'  '-'  '('  '-'(_.-' |  | \   |    |  '--'  /  #
#   `------'     `--'       `--'    `--' `--'      `-----'   `-----'    `--'  `--'    `-------'   #
#                                     LFIHOUND v1.0                                               #
#                                   Created By Zevuxo                                             #
###################################################################################################
 """

print(logo)

## Target Info
def Target_Info(url, base_wc):
	print("           =========TARGET INFO==========")
	print(f"[*] Target URL: {url}")
	colored()
	print(f"Base Word Count: {base_wc}")
	print("\n\n\n\n\n\n\n\n\n\n")




def fuzz(url, line):
	try:
			url = url + line ## append wordlist line to url 
			r = requests.get(url)
			wc = int(r.headers['Content-Length']) ## Send Request and capture content length
			

			## If Request Word-Count larger then base wordcount Succesful LFI
			if wc > base_wc:
				colored()
				print("Word count:", end=" ")
				cprint(f"{wc}", 'green', attrs=['bold'], end=" ")
				print(f"{' ' * (10 - len(str(wc)))}{url}") ## 

	except KeyboardInterrupt:
		print('[*] Good Bye')
		time.sleep(1)
		os.system("clear")
		exit()




## Just For Colored [*]
def colored():
	print("[",end="")
	cprint("*", 'green', attrs=['bold'], end="")
	print("]", end=" ")


if __name__ == '__main__':
	
	## argparse for command line arguments
	parser = argparse.ArgumentParser()
	parser.add_argument('-u', '--url', type=str, required=True, help="The URL To Fuzz")
	parser.add_argument("-w", '--wordlist', type=str, required=True, help="Wordlist To Use")
	args = parser.parse_args()

	url = args.url
	wordlist = args.wordlist


	file = open(wordlist, 'r').read().split('\n') ## open wordlist, readlines and split them
	
	s = requests.get(url)
	base_wc = int(s.headers["Content-Length"]) ## send first request to get the default content length so we know if the lfi was succesful

	
	Target_Info(url, base_wc)


	print("====================================================================")
	print("||   WORD-COUNT                                              URL  ||")
	print("====================================================================")
	for line in file:
		fuzz(url,line)
		