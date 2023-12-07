import socket
import struct
import sys
from binary_tree import Node, BinaryTree

BUF_SIZE = 1024
INT_SIZE = 4
NET_BYTE_ORDER = "big"


def recv_node_count(conn: socket.socket):
    return int.from_bytes(conn.recv(INT_SIZE), byteorder=NET_BYTE_ORDER)


def recv_node(conn: socket.socket):
    text_len = int.from_bytes(conn.recv(INT_SIZE), byteorder=NET_BYTE_ORDER)
    format = f"!ii{text_len}s"
    buf = conn.recv(text_len + 2 * INT_SIZE)
    left_child_idx, right_child_idx, text = struct.unpack(format, buf)
    text = text.decode()
    return left_child_idx, right_child_idx, text


def create_node(node_data, nodes):
    left_child_idx, right_child_idx, text = node_data
    node = Node(text)
    node.child_left = nodes[left_child_idx] if left_child_idx >= 0 else None
    node.child_right = nodes[right_child_idx] if right_child_idx >= 0 else None
    return node


def handle_connection(conn: socket.socket):
    nodes = []
    with conn:
        node_count = recv_node_count(conn)
        for _ in range(node_count):
            node_data = recv_node(conn)
            node = create_node(node_data, nodes)
            nodes.append(node)
    conn.close()
    print("Connection closed")
    tree = BinaryTree(nodes[-1])
    tree.print_tree_paths()


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
            print(f"Connection from: {addr}")
            handle_connection(conn)


if __name__ == "__main__":
    main()
