import heapq

inf = float('inf')
graph = [
    [inf,   19,     inf,    inf,    14,     inf,    18,     ],
    [19,    inf,    5,      7,      12,     inf,    inf,    ],
    [inf,   5,      inf,    3,      inf,    inf,    inf,    ],
    [inf,   7,      3,      inf,    8,      21,     inf,    ],
    [14,    12,     inf,    8,      inf,    inf,    16,     ],
    [inf,   inf,    inf,    21,     inf,    inf,    27,     ],
    [18,    inf,    inf,    inf,    16,     27,     inf,    ],
]

sym2idx = lambda c : (ord(c) - 98)
idx2sym = lambda x : chr(x + 98)

def kruskal(graph : list[list[int]]) -> list[list[int]]:
    n = len(graph)
    heap = []
    mst = [[] for _ in range(n)]

    for i in range(n):
        for j in range(i + 1, n):
            weight = graph[i][j]
            if weight != float('inf'):
                heapq.heappush(heap, (weight, i, j))

    U = set()
    while len(U) < n:
        w, v1, v2 = heapq.heappop(heap)
        if v1 in U and v2 in U:
            continue
        U.add(v1)
        U.add(v2)
        mst[v1].append(v2)
    
    return mst



mst = kruskal(graph)
print(mst)
