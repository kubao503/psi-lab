import socket
import struct
import flat


STR_LEN = 10
TIMEOUT = 2

class NetDictSender:
    def __init__(self, d: dict) -> None:
        self.dict = d
        self.packed_dict = None

    def __get_struct_format(self):
        pair_count = len(self.dict)
        return "!ii" + f"{STR_LEN}s" * 2 * pair_count

    def __pack_dict_to_struct(self):
        data_bytes = [bytes(d, "utf-8") for d in flat.flatten_dict(self.dict)]
        data_format = self.__get_struct_format()
        print("data format:", data_format)
        return struct.pack(data_format, len(self.dict), STR_LEN, *data_bytes)
    
    def __check_data_size(self, delivered_data_size):
        if delivered_data_size != len(self.packed_dict):
            expected = len(self.packed_dict)
            print(f"Expected {expected} bytes but got {delivered_data_size}")
            exit(1)
        print(f"Successfully sent {delivered_data_size} bytes")

    def sendto(self, address):
        self.packed_dict = self.__pack_dict_to_struct()
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.settimeout(TIMEOUT)
            try:
                sock.sendto(self.packed_dict, address)
                print(f"Packet sent")
                print(f"Waiting for acknowledgment for packet...")
                response = sock.recv(4)
                delivered_data_size = struct.unpack("!i", response)[0]
                self.__check_data_size(delivered_data_size)
            except socket.timeout:
                print(f"Timeout exceeded. Retransmitting packet ...")
                self.sendto(address)
            except Exception as e:
                print(f"An error occurred: {e}")
