import csv
import sys

def readcsv(filepath: str) -> list[tuple[int, list[int]]]:
    ret = []
    with open(filepath, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            digit = []
            for item in row:
                digit.append(int(item))
            ret.append((1, digit))
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
        elif (size == 1): return self.first_child().single_path()
        else: return False

    def print_tree(self, level=0):
        print(' ' * level * 4 + f'Node ID: {self.nid}, Support: {self.support}')
        for child in self.children.values():
            child.print_tree(level + 1)
    
    def first_child(self):
        children = list(self.children.values())
        if (children):
            return children[0]
        else:
            return None;

    def get_combinations(head):
        if not head:
            return [[]]
        rest_combinations = FPNode.get_combinations(head.first_child())
        current_combinations = [[head.nid] + combination for combination in rest_combinations]
        return current_combinations + rest_combinations 
    
    def find_min_support(self):
        min_sup = 1 << 31
        ptr = self.first_child()
        while (ptr):
            if ptr.support < min_sup:
                min_sup = ptr.support
            ptr = ptr.first_child()
        return min_sup

        

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

    def build(self, input):
        if isinstance(input, str):
            self.build_by_db(readcsv(input))
        else:
            self.build_by_db(input)

    def build_by_db(self, db: list[tuple[int, list[int]]]):
        self.database = db
        self.N = len(db)
        header_table = {}
        for support, itemset in db:
            current_node = self.root
            for nid in itemset:
                # build FP tree
                if nid in current_node.children.keys():
                    current_node.children[nid].support += support
                    header_table[nid][FPTree.TABLE_SUPPORT] += support
                else:
                    new_node = FPNode(nid, support)
                    new_node.parent = current_node
                    current_node.children[nid] = new_node
                    # build head table
                    if nid in header_table.keys():
                        header_table[nid][FPTree.TABLE_SUPPORT] += support
                        header_table[nid][FPTree.TABLE_TAIL].next = new_node
                        header_table[nid][FPTree.TABLE_TAIL] = new_node
                    else:
                        header_table[nid] = [nid, support, new_node, new_node]

                current_node = current_node.children[nid]

        self.header_table = sorted(header_table.values(), key = lambda entry : (entry[FPTree.TABLE_SUPPORT], -entry[FPTree.TABLE_NID]))

    def growth(self, min_sup):
        if isinstance(min_sup, float):
            min_sup = int(self.N * min_sup)
            print(min_sup)
        result = []
        FPTree.fp_growth(self, [], min_sup, result)
        return result

    def fp_growth(tree, a, min_sup, result):
        # print('Test tree', tree)
        # tree.print()
        if tree.root.single_path():
            combinations = FPNode.get_combinations(tree.root.first_child())
            support = tree.root.find_min_support()
            for b in combinations:
                result.append((support, a + b))
        else:
            for ai in tree.header_table:
                # filter who is not frequent
                support = ai[FPTree.TABLE_SUPPORT]
                if support < min_sup:
                    continue
                b = a.copy()
                b.append(ai[FPTree.TABLE_NID])
                result.append((support, b))
                # print('Test nid', ai[FPTree.TABLE_NID]);
                # go through link list
                ptr = ai[FPTree.TABLE_HEAD]
                prefix = []
                while (ptr):
                    # get node prefix
                    cpb = FPTree.cond_pattern(ptr)
                    if (cpb):
                        prefix.append(cpb)
                        # print('get', cpb)
                    ptr = ptr.next
                
                if (prefix):
                    treeb = FPTree()
                    treeb.build_by_db(prefix)
                    if (treeb.root.children):
                        FPTree.fp_growth(treeb, b.copy(), min_sup, result)

                
                


    def cond_pattern(ptr: FPNode):
        # if (ptr.parent.nid == FPTree.ROOT_NID):
        #     return None
        # if (ptr.parent.nid == FPTree.ROOT_NID):
        #     return (ptr.support, [ptr.nid])
        prefix = []
        support = ptr.support;
        ptr = ptr.parent; 
        while (ptr.nid != FPTree.ROOT_NID):
            prefix.append(ptr.nid)
            ptr = ptr.parent
        return (support, prefix[::-1])


    def print(self):
        self.root.print_tree()
        print()
        self.print_header_table()

    def print_header_table(self):
        for item in self.header_table:
            print("(nid: {}, sup: {})".format(item[FPTree.TABLE_NID], item[FPTree.TABLE_SUPPORT]), end=' ')
            ptr = item[FPTree.TABLE_HEAD]
            while (ptr) :
                print('-> (sup: {})'.format(ptr.support), end=' ')
                ptr = ptr.next;
            print()



def mine(csvfile, min_sup):
    tree = FPTree()
    tree.build(csvfile)
    result = tree.growth(min_sup)
    return sorted(result, key = lambda entry : -entry[0])
    
def print_maximal(n, mine_result):
    if n > len(mine_result):
        n = len(mine_result)
    for entry in mine_result[:n]:
        print(entry[0], '\t', entry[1])

if __name__ == '__main__':
    assert len(sys.argv) >= 3
    file = sys.argv[1]
    min_support = float(sys.argv[2])
    print(mine(file, min_support))