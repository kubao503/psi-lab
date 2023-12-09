import socket
import struct
import sys
from binary_tree import Node, BinaryTree
import select

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
        port = int(sys.argv[1])

    print(f"Will listen on {host}:{port}")

    # Gniazdo IPv4
    sock_ipv4 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock_ipv4.bind((host, port))
    sock_ipv4.listen()

    # Gniazdo IPv6
    sock_ipv6 = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
    sock_ipv6.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_V6ONLY, 0)
    sock_ipv6.bind((host, port))
    sock_ipv6.listen()

    print(host, port)
    print("IPv4 and IPv6 servers are listening...")

    while True:
        try:
            ready, _, _ = select.select([sock_ipv4, sock_ipv6], [], [])
            
            for sock in ready:
                connection, address = sock.accept()
                if sock == sock_ipv4:
                    print(f"Connection from (IPv4): {address}")
                else:
                    print(f"Connection from (IPv6): {address}")
                handle_connection(connection)

        except socket.error:
            pass


if __name__ == "__main__":
    main()
