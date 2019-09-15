template<TotallyOrdered T>
const T& min(const T& x, const T& y) {
    return min(x, y, std::less<T>());
}

template<TotallyOrdered T>
T& min(T& x, T& y) {
    return min(x, y, std::less<T>());
}
