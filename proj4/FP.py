import csv

def readcsv(filepath: str) -> list[list[int]]:
    ret = []
    with open(filepath, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            digit = []
            for item in row:
                digit.append(int(item))
            ret.append(digit)
    return ret
        

class FPNode:
    def __init__(self, nid, support):
        self.nid = nid;
        self.support = support
        self.next = None
        self.parent = None
        self.children = {}
        self.visited = False

    def single_path(self):
        size = len(self.children)
        if (size == 0): return True
        elif (size == 1): return self.children[0][1].single_path()
        else: return False

    def print_tree(self, level=0):
        print(' ' * level * 4 + f'Node ID: {self.nid}, Support: {self.support}')
        for child in self.children.values():
            child.print_tree(level + 1)

        

class FPTree:
    ROOT_NID = -1
    TABLE_NID = 0
    TABLE_SUPPORT = 1
    TABLE_HEAD = 2
    TABLE_TAIL = 3

    def __init__(self):
        self.root = FPNode(FPTree.ROOT_NID, 0)
        self.database = []
        self.header_table = []  # [nid, support, head, tail]


    def build(self, csvfile: str):
        self.database = readcsv(csvfile)
        header_table = {}
        for itemset in self.database:
            current_node = self.root
            for nid in itemset:
                # build FP tree
                if nid in current_node.children.keys():
                    current_node.children[nid].support += 1
                else:
                    new_node = FPNode(nid, 1)
                    new_node.father = current_node
                    current_node.children[nid] = new_node
                    # build head table
                    if nid in header_table.keys():
                        header_table[nid][FPTree.TABLE_SUPPORT] += 1
                        header_table[nid][FPTree.TABLE_TAIL].next = new_node
                        header_table[nid][FPTree.TABLE_TAIL] = new_node
                    else:
                        header_table[nid] = [nid, 1, new_node, new_node]

                current_node = current_node.children[nid]

        self.header_table = sorted(header_table.items(), key = lambda entry : entry[FPTree.TABLE_SUPPORT])



    def print_header_table(self):
        for item in self.header_table:
            print("(nid: {}, sup: {}".format(item[FPTree.TABLE_NID], item[FPTree.TABLE_SUPPORT]), end=' ')
            ptr = item[FPTree.TABLE_HEAD]
            while (ptr) :
                print('-> (sup: {})'.format(ptr.support), end=' ')
            print()


tree = FPTree()
tree.build('test\\small_test.csv')
tree.root.print_tree()
tree.print_header_table()

