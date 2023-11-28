import socket
import struct
import flat


STR_LEN = 10
TIMEOUT = 2

class NetDictSender:
    _dictobj = None
    _packed_dict = None

    def __init__(self, d: dict) -> None:
        self.dictobj = d

    def __get_struct_format(self):
        pair_count = len(self.dictobj)
        return "!ii" + f"{STR_LEN}s" * 2 * pair_count

    def __pack_dict_to_struct(self):
        data_bytes = [bytes(d, "utf-8") for d in flat.flatten_dict(self.dictobj)]
        data_format = self.__get_struct_format()
        print("data format:", data_format)
        return struct.pack(data_format, len(self.dictobj), STR_LEN, *data_bytes)

    def __check_response(self, response):
        delivered_data_size = struct.unpack("!i", response)[0]
        if delivered_data_size != len(self._packed_dict):
            expected = len(self._packed_dict)
            print(f"Expected {expected} bytes but got {delivered_data_size}")
            exit(1)
        print(f"Successfully sent {delivered_data_size} bytes")

    @property
    def dictobj(self):
        return self._dictobj

    @dictobj.setter
    def dictobj(self, obj):
        self._dictobj = obj
        self._packed_dict = self.__pack_dict_to_struct()

    def sendto(self, sock, address):
        sock.settimeout(TIMEOUT)
        try:
            sock.sendto(self._packed_dict, address)
            print(f"Packet sent")
            print("Waiting for confirmation...")
            response = sock.recv(4)
            self.__check_response(response)
        except socket.timeout:
            print(f"Timeout exceeded. Retransmitting packet ...")
            self.sendto(sock, address)
        except Exception as e:
            print(f"An error occurred: {e}")
