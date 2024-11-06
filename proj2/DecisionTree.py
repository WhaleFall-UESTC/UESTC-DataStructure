import numpy as np
from scipy import stats

eps = 1e-5      # a small number

class DecisionTree:
    def __init__(self, max_depth = 3, feature_labels = None, m = None):
        self.max_depth = max_depth      # max depth of the tree
        self.features = feature_labels   # names of features
        self.left = None
        self.right = None
        self.split_idx = None   # decide which feature to split
        self.thresh = None      # for splitting
        self.data = None        # feature values reach the leaf
        self.pred = None        # result
        self.m = m    # size of the randomly selected feature subset


    @staticmethod
    def calculate_entropy(y):
        probabilities = []

        for class_label in np.unique(y):
            count = len(y[np.where(y == class_label)])
            probabilities.append(float(count / len(y)))
        
        H_S = -1 * sum([pi * np.log2(pi) for pi in probabilities])
        return H_S


    @staticmethod
    def information_gain(X, y, thresh):
        H_S = DecisionTree.calculate_entropy(y)
        # get left and right subset labels after splitting
        S_l_y = y[np.where(X < thresh)]
        S_r_y = y[np.where(X >= thresh)]

        H_S_l = DecisionTree.calculate_entropy(S_l_y)
        H_S_r = DecisionTree.calculate_entropy(S_r_y)
        
        # H_S after splitting
        H_after = (len(S_l_y) * H_S_l + len(S_r_y) * H_S_r) / len(y)

        return H_S - H_after


    def split_test(self, X, idx, thresh):
        # idxx is a ndarray containing rows idx
        idx0 = np.where(X[:, idx] < thresh)[0]
        idx1 = np.where(X[:, idx] >= thresh)[0]
        # X0, X1 are matrixs
        X0, X1 = X[idx0, :], X[idx1, :]
        return X0, idx0, X1, idx1

    
    def split(self, X, y, idx, thresh):
        X0, idx0, X1, idx1 = self.split_test(X, idx, thresh)
        y0, y1 = y[idx0], y[idx1]
        return X0, y0, X1, y1

    
    def fit(self, X, y):
        if self.max_depth == 0:
            # reach max depth
            self.data = X
            self.labels = y
            # find the most frequent in y
            self.pred = stats.mode(y).mode[0]
            return self
        
        gains = []
        original_X = X

        if self.m:
            # if m is set, randomly chose m features
            attribute_bag = np.random.choice(list(range(len(self.features))), size = self.m, replace = False)
            X = original_X[:, attribute_bag]
        else:
            attribute_bag = None
            X = original_X

        thresh = np.array([
            # generate threshold of 10 linear intervals for each feature
            np.linspace(np.min(X[:, i]) + eps, np.max(X[:, i] - eps, num = 10))
            # X.shape[1]: number of columns
            for i in range(X.shape[1])
        ])

        for i in range(X.shape[1]):
            gains.append([self.information_gain(X[:, i], y, t) for t in thresh[i, :]])

        gains = np.nan_to_num(np.array(gains))

        # find split_idx and thresh_idx with max inform_gain
        self.split_idx, thresh_idx = np.unravel_index(np.argmax(gains), gains.shape)
        self.thresh = thresh[self.split_idx, thresh_idx]

        if self.m:
            self.split_idx = attribute_bag[self.split_idx]

        X0, y0, X1, y1 = self.split(original_X, y, self.split_idx, self.thresh)
        
        if X0.size > 0 and X1.size > 0:
            self.left = DecisionTree(
                max_depth = self.max_depth - 1, 
                feature_labels = self.features,
                m = self.m
            )
            self.left.fit(X0, y0)
            self.right = DecisionTree(
                max_depth = self.max_depth - 1,
                feature_labels = self.features,
                m = self.m
            )
            self.right.fit(X1, y1)
        else:
            self.max_depth = 0
            self.data = original_X
            self.labels = y
            self.pred = stats.mode(y).mode[0]

        return self


    def predict(self, X, verbose = False):
        if self.max_depth == 0:
            # return prediction
            return self.pred * np.ones(X.shape[0])
        else:
            if (verbose and X.shape[0] != 0):
                print(
                    "feature", self.features[self.split_idx],
                    "value", X[0, self.split_idx],
                    ">/<", self.thresh
                )
            
            X0, idx0, X1, idx1 = self.split_test(X, self.split_idx, self.thresh)

            yhat = np.zeros(X.shape[0])
            yhat[idx0] = self.left.predict(X0, verbose=verbose)
            yhat[idx1] = self.right.predict(X1, verbose=verbose)

            return yhat
        


class RandomForest:
    def __init__(self, max_depth=3, n=25, featrues=None, sample_size=None):
        self.n = n
        self.sample_size = sample_size
        self.decision_trees = [
            DecisionTree(max_depth=max_depth, feature_labels=featrues)
            for i in range(n)
        ]


    def fit(self, X, y):
        assert self.sample_size < len(y), "Sample size must be less than test size"

        connect = np.concatenate((X, y.reshape(-1,1)), axis=1)

        for tree in self.decision_trees:
            samples = np.random.choice(list(range(len(connect))), size=self.sample_size, replace=True)
            
            train_data = connect[samples, :]
            train_X = train_data[:, :-1]
            train_y = train_data[:, -1:]
            
            tree.fit(train_X, train_y)

    
    def predict(self, X):
        predictions = []
        for tree in self.decision_trees:
            predictions.append(tree.predict(X))

        total_pred = np.vstack(predictions);
        model_prediction = stats.mode(total_pred).mode[0]

        return model_prediction

