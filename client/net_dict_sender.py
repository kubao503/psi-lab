import socket
import struct
import flat

STR_LEN = 10


class NetDictSender:
    def __init__(self, d: dict) -> None:
        self.dict = d
        self.packed_dict = self.__pack_dict_to_struct()

    def __get_struct_format(self):
        pair_count = len(self.dict)
        return "!ii" + f"{STR_LEN}s" * 2 * pair_count

    def __pack_dict_to_struct(self):
        data_bytes = [bytes(d, "utf-8") for d in flat.flatten_dict(self.dict)]
        data_format = self.__get_struct_format()
        print("data format:", data_format)
        return struct.pack(data_format, len(self.dict), STR_LEN, *data_bytes)

    def __check_response(self, response):
        delivered_data_size = struct.unpack("!i", response)[0]
        if delivered_data_size != len(self.packed_dict):
            expected = len(self.packed_dict)
            print(f"Expected {expected} bytes but got {delivered_data_size}")
            exit(1)
        print(f"Successfully sent {delivered_data_size} bytes")

    def sendto(self, address):
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.sendto(self.packed_dict, address)
            print("Waiting for confirmation...")
            response = sock.recv(4)
        self.__check_response(response)
