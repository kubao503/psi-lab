import socket
import flat
import struct

BUFSIZE = 1024
HEADER_SIZE = 12


class NetDictReceiver:
    def __init__(self, address) -> None:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind(address)
        self.socket = s

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.socket.close()

    def __unpack_header(self):
        pair_count, str_len, packet_number = struct.unpack(
            "iii", self.packed_dict[:HEADER_SIZE]
        )
        self.pair_count = pair_count
        self.str_len = str_len
        self.packet_number = packet_number

    def __get_data_format(self):
        return f"{self.str_len}s" * 2 * self.pair_count

    def __unpack_dict(self):
        data_format = self.__get_data_format()
        print("data format:", data_format)

        data_bytes = struct.unpack(data_format, self.packed_dict[HEADER_SIZE:])
        flat_dict = [d.decode("utf-8").rstrip("\x00") for d in data_bytes]
        self.dict = flat.build_dict(flat_dict)

    def __send_response(self):
        response = struct.pack("ii", len(self.packed_dict), self.packet_number)
        self.socket.sendto(response, self.address)

    def recv(self):
        data, address = self.socket.recvfrom(BUFSIZE)
        if not data:
            print("Error in datagram?")
            return
        print(f"Received datagram from {address[0]}:{address[1]}")
        self.packed_dict = data
        self.address = address
        self.__unpack_header()
        self.__unpack_dict()
        self.__send_response()

    def get(self):
        return self.dict
