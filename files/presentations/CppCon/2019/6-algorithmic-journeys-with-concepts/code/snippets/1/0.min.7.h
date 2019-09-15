template<Regular T, Relation r>
const T& min(const T& x, const T& y, Relation r) {
    if (r(y, x)) { return y; }
    return x;
}

template<Regular T, Relation r>
T& min(T& x, T& y, Relation r) {
    if (r(y, x)) { return y; }
    return x;
}
