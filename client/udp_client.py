# simple Python socket client
# (c) G.Blinowski for PSI 2021

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

datagram = NetDictSender(data)
for _ in range(5):
    datagram.sendto((HOST, port))

print("Client finished.")
