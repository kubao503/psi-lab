import socket
import struct
import sys

BUF_SIZE = 1024
INT_SIZE = 4


def main():
    host = socket.gethostname()

    if len(sys.argv) < 2:
        port = 8000
    else:
        port = sys.argv[1]

    print(f"Will listen on {host}:{port}")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind((host, port))
        sock.listen()
        while True:
            conn, addr = sock.accept()
            with conn:
                print(f"Connection from: {addr}")
                packet_count = int.from_bytes(
                    conn.recv(INT_SIZE), sys.byteorder
                )
                for _ in range(packet_count):
                    text_len = int.from_bytes(
                        conn.recv(INT_SIZE), sys.byteorder
                    )
                    format = f"=ii{text_len}s"
                    buf = conn.recv(text_len + 2 * INT_SIZE)
                    left_child, right_child, text = struct.unpack(format, buf)
                    print(text, left_child, right_child)
            conn.close()
            print("Connection closed by client")


if __name__ == "__main__":
    main()
