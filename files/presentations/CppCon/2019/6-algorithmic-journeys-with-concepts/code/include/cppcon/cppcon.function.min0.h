namespace cppcon {

template<totally_ordered T>
const T& min(const T& x, const T& y) {
    if (y < x) {
        return y;
    }
    return x;
}

template<totally_ordered T>
T& min(T& x, T& y) {
    if (y < x) {
        return y;
    }
    return x;
}

}
