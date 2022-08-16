import numpy as np
import random
from math import log, log2
from itertools import combinations, islice, groupby
from random import random as random_float
from sklearn.datasets import load_svmlight_file


def ascending_descending_key(x):
    assert len(x) == 2
    return (x[0], -x[1])


def make_subranges_from_fractions(n, fractions):
    i = 0
    for k, fraction in enumerate(fractions, 1):
        if k == len(fractions):
            yield i, n
        else:
            delta = int(fraction * n)
            yield i, i + delta
            i += delta
            
def split_df_by_fractions(df, group_id, fractions=[0.5, 0.5], seed=None):
    fractions = np.array(fractions)
    fractions = fractions / fractions.sum()
    groups = df[group_id].unique()
    np.random.seed(seed)
    np.random.shuffle(groups)
    
    return [df[df[group_id].isin(groups[i:j])] for i, j in make_subranges_from_fractions(len(groups), fractions)]


def grouped_ltr_output(df, k, group_id_column="138"):
    return [df[k].values for _, df in df.groupby(group_id_column)]


def shuffle(x):
    #print(x)
    x = list(x)
    random.shuffle(x)
    return x

def shuffle_truncate(count):
    def wrapper(x):
        return shuffle(x)[:count]
    return wrapper

def select_ith_element(a, i):
    return [x[i] for x in a]


def generate_pairs_from_rows(xyg, max_group_size=None, permute_pairs=lambda x: x):
    for gid, rows in groupby(xyg, key=lambda x: x[-1]):
        count = 0
        for r0, r1 in permute_pairs(combinations(rows, 2)):
            y0, y1 = r0[-2], r1[-2]
            if y0 < y1:
                yield (r0[:-2], y0), (r1[:-2], y1)
                count += 1
            elif y1 < y0:
                yield (r1[:-2], y1), (r0[:-2], y0)
                count += 1
            if count == max_group_size:
                break


def generate_ltr_pairs(xyg,
                       transformation=lambda x: x[0],
                       max_group_size=None,
                       permute_pairs=lambda x: x):

    for gid, rows in groupby(xyg, key=lambda x: x[1][-1]):
        count = 0
        for r0, r1 in permute_pairs(combinations(rows, 2)):
            y0, y1 = r0[1][-2], r1[1][-2]
            if y0 < y1:
                yield transformation(r1), transformation(r0)
                count += 1
            elif y1 < y0:
                yield transformation(r0), transformation(r1)
                count += 1
            if count == max_group_size:
                break
            

        
    
def generate_pairs(X, y, group_id, max_group_size=None):
    for gid, rows in groupby(zip(X, y, group_id), key=lambda x: x[-1]):
        count = 0
        for (x0, y0, _), (x1, y1, _) in combinations(rows, 2):
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

        if self.use_weights:
            self.svc = self.svc.fit(self.x_train, self.y_train, sample_weight=weights)
        else:
            self.svc = self.svc.fit(self.x_train, self.y_train)
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


def ngrams(iterable, n, dtype=tuple, transformation=None):
    if f is None:
        ngram = [x for x in islice(iterable, 0, n)]
        if len(ngram) != n:
            return
        yield dtype(ngram)

        for x in islice(iterable, n, None):
            ngram.pop(0)
            ngram.append(x)
            yield dtype(ngram)
        return

    if callable(f):
        ngram = [f(x) for x in islice(iterable, 0, n)]
        if len(ngram) != n:
            return
        yield dtype(ngram)

        for x in islice(iterable, n, None):
            ngram.pop(0)
            ngram.append(f(x))
            yield dtype(ngram)
        return
    raise ValueError("f must be None or callable")


def ngrams_with_left_pad(iterable, n, pad="", dtype=tuple, f=None):
    ngrams = [pad for x in range(n)]

    if f is not None:
        for x in iterable:
            ngrams.pop(0)
            ngrams.append(x)
            yield dtype(ngrams)
        return None

    if callable(f):
        for x in iterable:
            ngrams.pop(0)
            ngrams.append(f(x))
            yield dtype(ngrams)
        return None
    raise ValueError("f must be None or callable")
    

def dcg(relevances, n=None):
    if n is None:
        n = len(relevances)
    return sum(x / log2(i) for i, x in islice(enumerate(relevances, 2), 0, n))


def good_pair(x, y):
    return (x[0] < y[0] and x[1] < y[1]) or (x[0] > y[0] and x[1] > y[1])


def pair_accuracy(y_trues, y_preds):
    s = 0.0
    total_count = 0
    for y_true, y_pred in zip(y_trues, y_preds):
        if len(y_true) > 1:
            for x, y in combinations(list(zip(y_true, y_pred)), 2):
                # Ignore pairs with the same relevance in F8
                if x[0] != y[0]:
                    s += good_pair(x, y)
                    total_count += 1
    return s / total_count


def dcg_exp(relevances, n=None):
    if n is None:
        n = len(relevances)
    return sum((2.0**x - 1.0) / log2(i) for i, x in islice(enumerate(relevances, 2), 0, n))


def zip_sort(*args, **kwargs):
    pairs = list(zip(*args))
    pairs.sort(**kwargs)
    return pairs


def _ndcg(y_true, y_pred, dcg, n=None, copy=True):
    assert len(y_true) == len(y_pred)

    if n is None:
        n = len(y_pred)

    a = list(zip(y_pred, y_true))
    a.sort(reverse=True, key=ascending_descending_key)

    if copy:
        y_true = list(y_true)
    y_true.sort(reverse=True)

    m = dcg((x[1] for x in a), n=n)
    n = dcg(y_true, n=n)

    if n > 0:
        return m / n

    if m == n:
        return 1

    return 0.0

def ndcg(y_true, y_pred, n=None, copy=True):
    return _ndcg(y_true, y_pred, dcg, n=n, copy=copy)



def mean_ndcg(y_trues, y_preds, n=None):
    return np.mean([ndcg(y_true, y_pred, n=n) for y_true, y_pred in zip(y_trues, y_preds)])

def mean_ndcg_exp(y_trues, y_preds, n=None):
    return np.mean([ndcg_exp(y_true, y_pred, n=n) for y_true, y_pred in zip(y_trues, y_preds)])

def ndcg_exp(y_true, y_pred, n=None, copy=True):
    return _ndcg(y_true, y_pred, dcg_exp, n=n, copy=copy)

def main():
    x_train, y_train, group_id_train = load_ltr_dataset("data/MSLR-WEB10K/Fold1/tmp") 
    x_test, y_test, group_id_test = load_ltr_dataset("data/MSLR-WEB10K/Fold1/test.txt") 

    linear_svc = LinearSVC()
    ranker = SVCRanker(linear_svc)
    ranker.fit(x_train, y_train, group_id_train)
    import pdb;pdb.set_trace()  

def make_pairwise_classifier(X, pairs, seed):
    X_train = X[pairs[:, 0]] - X[pairs[:, 1]]
    y_train = np.ones(X_train.shape[0])
    indicies = np.arange(X_train.shape[0])
    np.random.seed(seed)
    np.random.shuffle(indicies)
    indicies = indicies[:X_train.shape[0] // 2]
    X_train[indicies] *= -1
    y_train[indicies] *= -1
    return X_train, y_train



class PairwiseRanker(object):
    def __init__(self, binary_classifier, seed=0):
        self.binary_classifier = binary_classifier
        self.seed = seed

    def fit(self, X, y, pairs=None):
        X_train, y_train = make_pairwise_classifier(X, pairs, self.seed)
        self.binary_classifier.fit(X_train, y_train)
        return self

    def predict(self, X):
        return self.binary_classifier.decision_function(X)

        

if __name__ == "__main__":
    main()
