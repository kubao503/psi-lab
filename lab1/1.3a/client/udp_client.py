# simple Python socket client
# (c) G.Blinowski for PSI 2021

import socket
import math
import sys
from net_dict_sender import NetDictSender


HOST = "server"

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will send to ", HOST, ":", port)

data = {}

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    datagram = NetDictSender(data)
    for i in range(100):
        num_elements = math.ceil(9570 * 0.5**(i/2)) # liczba ile ma być dodanych kluczy-wartości
        last_num = len(data)
        print(num_elements)
        data.update({f"{last_num+j:03d}": f"{last_num+j:03d}" for j in range(num_elements)}) # dodawanie nowych elementów (wyrównanie do 3 cyfry)
        datagram.dictobj = data
        datagram.sendto(sock, (HOST, port))

print("Client finished.")
