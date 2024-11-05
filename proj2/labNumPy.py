import numpy as np

X = np.array([
    [1, 2, 3, 4],
    [5, 6, 7, 8], 
    [9, 10, 11, 12], 
    [13, 14, 15, 16] 
])

''' Slicing Test '''
print(X[1, :])
# [5 6 7 8]
print(X[1, 1:3])
# [6 7]
print(X[0:2, 0:2])
# [[1 2]
#  [5 6]]
idx = np.array([1, 2, 2, 2, 2, 3, 3, 3, 3])
print(X[np.unique(idx), :])
# [[ 5  6  7  8]
#  [ 9 10 11 12]
#  [13 14 15 16]]
chose = np.array([0, 1, 5, 8])
print(chose[idx])
# [1 5 5 5 5 8 8 8 8]
print(idx[chose])
# [1 2 3 3]


''' np.where(condition) '''
print(np.where(X > 7))
print(np.where(X > 7)[0], type(np.where(X > 7)[0]))
print(np.where(X > 7)[1], type(np.where(X > 7)[1]))
# (array([1, 2, 2, 2, 2, 3, 3, 3, 3]), array([3, 0, 1, 2, 3, 0, 1, 2, 3]))
# [1 2 2 2 2 3 3 3 3] <class 'numpy.ndarray'>
# [3 0 1 2 3 0 1 2 3] <class 'numpy.ndarray'>
# 元组的第一个元素是所有大于 7 的元素的行标


''' np.random.choice '''
print(np.random.choice(list(range(100)), size = 10, replace = False))
# [52  1 79 57 94 43 92 45 69 29]


''' np.linspace '''
print(np.linspace(10, 100, num = 9))
print(np.linspace(10, 200, num = 15))
# [ 10.    21.25  32.5   43.75  55.    66.25  77.5   88.75 100.  ]
# [ 10.          23.57142857  37.14285714  50.71428571  64.28571429
#   77.85714286  91.42857143 105.         118.57142857 132.14285714
#  145.71428571 159.28571429 172.85714286 186.42857143 200.        ]


''' np.argmax '''
nda = np.array([1, 3, 5, 9, 24, 0, 2, 8])   # nda[4] = 24 = max
print(np.argmax(nda))   # 4
print(np.argmax(X))     # 15 :index of the matrix flattern to list


''' np.unravel_index '''
print(np.unravel_index(15, X.shape))
# (np.int64(3), np.int64(3))   matrix index (3, 3)


