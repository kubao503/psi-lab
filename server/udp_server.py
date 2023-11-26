# simple Python socket UDP server
# (C) G.Blinowski for PSI 2021

import socket
import sys
import struct

HOST = socket.gethostname()
BUFSIZE = 1024

if len(sys.argv) < 2:
    print("no port, using 8000")
    port = 8000
else:
    port = int(sys.argv[1])

print("Will listen on ", HOST, ":", port)


with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind((HOST, port))
    while True:
        data, address = s.recvfrom(BUFSIZE)
        if not data:
            print("Error in datagram?")
            break
        print(f"Received datagram from {address[0]}:{address[1]}")

        pair_count, str_len = struct.unpack("!ii", data[:8])
        print("pair count", pair_count, "str len", str_len)
        data_format = f"{str_len}s" * pair_count
        print("data format", data_format)
        data_bytes = struct.unpack(data_format, data[8:])
        decoded_data = [d.decode("utf-8").rstrip("\x00") for d in data_bytes]
        print(decoded_data)
