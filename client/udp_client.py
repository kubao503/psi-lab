# simple Python socket client
# (c) G.Blinowski for PSI 2021

import socket
import sys
from net_dict_sender import NetDictSender


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

    for _ in range(5):
        datagram.sendto(sock, (HOST, port))

print("Client finished.")
