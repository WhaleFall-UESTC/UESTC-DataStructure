class TreeNode:
    def __init__(self, key, left=None, right=None):
        self.key = key
        self.left = left
        self.right = right
        self.height = 1

class AVLTree:
    def insert(self, root, key):
        # 1. Perform the normal BST insertion
        if not root:
            return TreeNode(key)
        elif key < root.key:
            root.left = self.insert(root.left, key)
        elif key > root.key:
            root.right = self.insert(root.right, key)
        else:  # Equal keys are not allowed in BST
            return root

        # 2. Update height of this ancestor node
        root.height = 1 + max(self.getHeight(root.left),
                              self.getHeight(root.right))

        # 3. Get the balance factor of this ancestor node to check whether
        # this node became unbalanced
        balance = self.getBalance(root)

        # If this node becomes unbalanced, then there are 4 cases

        # Left Left Case 往左走（左子树更高），然后再往左走（key 比左子树 key 小）
        if balance > 1 and key < root.left.key:
            return self.rightRotate(root)
        # LL 右旋

        # Right Right Case 往右走，在向右走
        if balance < -1 and key > root.right.key:
            return self.leftRotate(root)
        # RR 左旋

        # Left Right Case 往左走，然后转向右走
        if balance > 1 and key > root.left.key:
            root.left = self.leftRotate(root.left)
            return self.rightRotate(root)
        # LR 左旋左子树，再右旋根节点

        # Right Left Case 往右走，然后转向左走
        if balance < -1 and key < root.right.key:
            root.right = self.rightRotate(root.right)
            return self.leftRotate(root)
        # RL 右旋右子树，再左旋根节点

        return root

    # 左旋对右子树进行操作
    def leftRotate(self, z):
        y = z.right
        T2 = y.left

        # Perform rotation
        y.left = z
        z.right = T2

        # Update heights
        z.height = 1 + max(self.getHeight(z.left),
                           self.getHeight(z.right))
        y.height = 1 + max(self.getHeight(y.left),
                           self.getHeight(y.right))

        # Return the new root
        return y
        
    # 右旋对左子树进行操作
    def rightRotate(self, y):
        x = y.left
        T2 = x.right

        # Perform rotation
        x.right = y
        y.left = T2

        # Update heights
        y.height = 1 + max(self.getHeight(y.left),
                           self.getHeight(y.right))
        x.height = 1 + max(self.getHeight(x.left),
                           self.getHeight(x.right))

        # Return the new root
        return x

    def getHeight(self, root):
        if not root:
            return 0
        return root.height

    def getBalance(self, root):
        if not root:
            return 0
        return self.getHeight(root.left) - self.getHeight(root.right)

    def printTree(self, node, level=0, prefix="Root: "):
        if node is not None:
            print(" " * (level * 4) + prefix + str(node.key))
            if node.left or node.right:
                if node.left:
                    self.printTree(node.left, level + 1, "L--- ")
                else:
                    print(" " * ((level + 1) * 4) + "L--- None")
                if node.right:
                    self.printTree(node.right, level + 1, "R--- ")
                else:
                    print(" " * ((level + 1) * 4) + "R--- None")

# Example usage:
avl_tree = AVLTree()
root = None
keys = [35, 51, 30, 63, 72, 15, 8, 58, 46, 24]

for key in keys:
    root = avl_tree.insert(root, key)

# The AVL tree will be balanced after each insertion
avl_tree.printTree(root);