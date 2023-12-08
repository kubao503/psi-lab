import socket
import struct
import flat


STR_LEN = 10
TIMEOUT = 1.5

class NetDictSender:
    _dictobj = None
    _packed_dict = None
    _set_packet_numbers = None

    def __init__(self, d: dict) -> None:
        self.dictobj = d

    def __get_struct_format(self):
        pair_count = len(self.dictobj)
        return "iii" + f"{STR_LEN}s" * 2 * pair_count

    def __pack_dict_to_struct(self, packet_number):
        data_bytes = [bytes(d, "utf-8") for d in flat.flatten_dict(self.dictobj)]
        data_format = self.__get_struct_format()
        # print("data format:", data_format)
        return struct.pack(data_format, len(self.dictobj), STR_LEN, packet_number, *data_bytes)

    def __check_response(self, response):
        delivered_data_size, delivered_packet_number = struct.unpack("ii", response)
        if delivered_data_size != len(self._packed_dict):
            expected = len(self._packed_dict)
            print(f"Expected {expected} bytes but got {delivered_data_size}")
            exit(1)
        print(f"Received acknowledgement for packet {delivered_packet_number} with data size: {delivered_data_size} bytes")
        self._set_packet_numbers.remove(delivered_packet_number)

    @property
    def dictobj(self):
        return self._dictobj

    @dictobj.setter
    def dictobj(self, obj):
        self._dictobj = obj

    @property
    def set_packet_numbers(self):
        if self._set_packet_numbers is None:
            raise ValueError("set_packet_numbers not initialized")
        return self._set_packet_numbers

    @set_packet_numbers.setter
    def set_packet_numbers(self, numbers):
        self._set_packet_numbers = set(numbers)

    def sendto(self, sock, address, packets_number):
        self.set_packet_numbers = range(packets_number)
        sock.settimeout(TIMEOUT)
        while self.set_packet_numbers:
            for packet_number in set(self.set_packet_numbers):
                self._packed_dict = self.__pack_dict_to_struct(packet_number)
                try:
                    sock.sendto(self._packed_dict, address)
                    print(f"Packet {packet_number} sent. Waiting for confirmation...")
                    response = sock.recv(8)
                    self.__check_response(response)
                except socket.timeout:
                    print(f"Timeout exceeded. Will retransmit packet {packet_number} ...")
                except Exception as e:
                    print(f"An error occurred: {e}")
