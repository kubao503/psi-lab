# simple Python socket client
# (c) G.Blinowski for PSI 2021

import sys
from net_dict_sender import NetDictSender
import socket
import time

HOST = "server"

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will send to ", HOST, ":", port)

data = {"type": "text", "value": "bajojajo", "id": "1"}

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    datagram = NetDictSender(data)

    for packet_number in range(5):
        datagram.sendto(sock, (HOST, port), packet_number)
        time.sleep(1)



print("Client finished.")
