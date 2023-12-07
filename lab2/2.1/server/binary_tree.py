class Node:
    def __init__(self, text) -> None:
        self.child_left = None
        self.child_right = None
        self.text = text


class BinaryTree:
    def __init__(self, root: Node) -> None:
        self.root = root

    def __print_tree_paths_rec(self, node: Node, path_text: str):
        if node is None:
            return

        path_text += node.text

        if node.child_left is None and node.child_right is None:
            print(path_text)
            return

        self.__print_tree_paths_rec(node.child_left, path_text)
        self.__print_tree_paths_rec(node.child_right, path_text)

    def print_tree_paths(self):
        self.__print_tree_paths_rec(self.root, "")
