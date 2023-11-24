# simple Python socket UDP server
# (C) G.Blinowski for PSI 2021

import socket
import sys

HOST = socket.gethostname()
# BUFSIZE = 512
BUFSIZE = 1024

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will listen on ", HOST, ":", port)


with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind((HOST, port))
    i = 1
    while True:
        data, address = s.recvfrom(BUFSIZE)
        print("Message from Client:{}".format(data))
        print("Client IP Address:{}".format(address))

        if not data:
            print("Error in datagram?")
            break
        # echo back data - NOTE - compare send() and sendall()!
        s.sendto(data, address)
        print("sending dgram #", i)
        i += 1
