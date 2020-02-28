import itertools
import numpy as np
from random import random as random_float
from sklearn.datasets import load_svmlight_file


def generate_pairs(X, y, group_id, max_group_size=None):
    for gid, rows in itertools.groupby(zip(X, y, group_id), key=lambda x: x[-1]):
        count = 0
        for (x0, y0, _), (x1, y1, _) in itertools.combinations(rows, 2):
            if y0 != y1:
                if y0 < y1:
                    yield (x0, y0), (x1, y1)
                else:
                    yield (x1, y1), (x0, y0)
                count += 1
            if max_group_size is not None and count == max_group_size:
                break


class SVCRanker(object):
    def __init__(self, svc=None, max_group_size=1000, use_weights=True):
        self.svc = svc
        self.max_group_size = max_group_size
        self.use_weights = use_weights

    def fit(self, X, y, group_id=None):
        assert group_id is not None

        x_train = []
        y_train = []
        weights = []

        for (x0, y0), (x1, y1) in generate_pairs(X, y, group_id, max_group_size=self.max_group_size):
            assert y0 < y1
            features, label = generate_pair(x0, x1)
            x_train.append(features)
            y_train.append(label)
            weights.append(y1 - y0)

        self.x_train = np.array(x_train)
        self.y_train = np.array(y_train)

        self.svc = self.svc.fit(self.x_train, self.y_train, sample_weight=weights)
        return self

    def predict(self, X):
        return self.svc.decision_function(X)


def generate_pair(x0, x1):
    if random_float() > 0.5:
        return x1 - x0, 1
    return x0 - x1, 0


def load_ltr_dataset(path):
    x_train, y_train, group_id = load_svmlight_file(path, query_id="qid") 
    x_train = np.array(x_train.todense())
    sort_index = np.argsort(group_id)
    return x_train[sort_index], y_train[sort_index], group_id[sort_index]


def main():
    x_train, y_train, group_id_train = load_ltr_dataset("data/MSLR-WEB10K/Fold1/tmp") 
    x_test, y_test, group_id_test = load_ltr_dataset("data/MSLR-WEB10K/Fold1/test.txt") 

    linear_svc = LinearSVC()
    ranker = SVCRanker(linear_svc)
    ranker.fit(x_train, y_train, group_id_train)
    import pdb;pdb.set_trace()  

if __name__ == "__main__":
    main()
