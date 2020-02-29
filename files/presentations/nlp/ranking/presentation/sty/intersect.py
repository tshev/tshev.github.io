def intersect(x, y):
    if len(x) > len(y):
        x, y = y, x
    return [i for i in x if not in y]
