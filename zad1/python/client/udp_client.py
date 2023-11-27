# simple Python socket client
# (c) G.Blinowski for PSI 2021

import socket
import sys
import struct
import flat


HOST = "server"
STR_LEN = 10

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will send to ", HOST, ":", port)

data = {"type": "text", "value": "bajojajo", "id": "1"}


with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    pair_count = len(data)
    data_format = "!ii" + f"{STR_LEN}s" * 2 * pair_count
    print("data format", data_format)
    print(flat.flatten_dict(data))

    data_bytes = [bytes(d, "utf-8") for d in flat.flatten_dict(data)]
    print(len(data_bytes), *data_bytes)

    packed_data = struct.pack(data_format, pair_count, STR_LEN, *data_bytes)
    for _ in range(5):
        s.sendto(packed_data, (HOST, port))

print("Client finished.")