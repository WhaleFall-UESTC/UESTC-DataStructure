class Tree:
    def __init__(self, val, left, right):
        self.val = val
        self.left = left
        self.right = right

global_cnt = 0
def traversal(tree):
    global global_cnt
    if (tree == None):
        return
    if (tree.left == None and tree.right == None):
        print(tree.val, ':', bin(global_cnt))
        global_cnt += 1
    else:
        traversal(tree.left)
        traversal(tree.right)
    
    

lst_str = input().split(' ')
lst = []
for i in lst_str:
    lst.append(Tree(int(i), None, None))

while len(lst) > 1:
    min1 = min(lst, key=lambda tree: tree.val)
    lst.remove(min1)
    min2 = min(lst, key=lambda tree: tree.val)
    lst.remove(min2)
    lst.append(Tree(min1.val + min2.val, min1, min2))

traversal(lst[0])
