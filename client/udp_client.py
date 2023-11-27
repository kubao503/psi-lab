# simple Python socket client
# (c) G.Blinowski for PSI 2021

import sys
from net_dict import NetDict


HOST = "server"

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will send to ", HOST, ":", port)

data = {"type": "text", "value": "bajojajo", "id": "1"}

datagram = NetDict(data)
datagram.sendto((HOST, port))

print("Client finished.")
