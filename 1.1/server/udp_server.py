# simple Python socket UDP server
# (C) G.Blinowski for PSI 2021

import socket
import sys

from net_dict_receiver import NetDictReceiver

HOST = socket.gethostname()

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will listen on ", HOST, ":", port)


with NetDictReceiver((HOST, port)) as net_dict:
    while True:
        net_dict.recv()
        print(net_dict.get())
