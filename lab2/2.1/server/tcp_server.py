import socket
import sys

BUF_SIZE = 1024


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
                print(f"Connect from: {addr}")
                while True:
                    data = conn.recv(BUF_SIZE)
                    if not data:
                        break
            conn.close()
            print("Connection closed by client")


if __name__ == "__main__":
    main()
