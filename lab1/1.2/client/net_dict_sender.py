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
        return "iii" + f"{STR_LEN}s" * 2 * pair_count

    def __pack_dict_to_struct(self, packet_number):
        data_bytes = [
            bytes(d, "utf-8") for d in flat.flatten_dict(self.dictobj)
        ]
        data_format = self.__get_struct_format()
        print("data format:", data_format)
        return struct.pack(
            data_format, len(self.dictobj), STR_LEN, packet_number, *data_bytes
        )

    def __check_response(self, response):
        delivered_data_size, delivered_packet_number = struct.unpack(
            "ii", response
        )
        if delivered_data_size != len(self._packed_dict):
            expected = len(self._packed_dict)
            print(f"Expected {expected} bytes but got {delivered_data_size}")
            exit(1)
        print(
            f"Packet {delivered_packet_number} was successfully sent with {delivered_data_size} bytes"
        )

    @property
    def dictobj(self):
        return self._dictobj

    @dictobj.setter
    def dictobj(self, obj):
        self._dictobj = obj

    def sendto(self, sock, address, packet_number):
        self._packed_dict = self.__pack_dict_to_struct(packet_number)
        sock.settimeout(TIMEOUT)
        while True:
            try:
                sock.sendto(self._packed_dict, address)
                print("Packet sent")
                print("Waiting for confirmation...")
                response = sock.recv(8)
                self.__check_response(response)
                break
            except socket.timeout:
                print("Timeout exceeded. Retransmitting packet ...")
            except Exception as e:
                print(f"An error occurred: {e}")
