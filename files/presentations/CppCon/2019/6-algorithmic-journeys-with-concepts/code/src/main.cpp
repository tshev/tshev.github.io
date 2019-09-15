#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <type_traits>
#include <cppcon/cppcon.h>

// TODO: properties for relations

namespace cppcon {

template<cppcon::totally_ordered T>
const T& min(const T& x, const T& y) {
    if (y < x) {
        return y;
    }
    return x;
}

template<cppcon::totally_ordered T>
T& min(T& x, T& y) {
    if (y < x) {
        return y;
    }
    return x;
}

template<cppcon::regular T, cppcon::relation<T> R>
const T& min(const T& x, const T& y, R r) {
    if (r(y, x)) {
        return y;
    }
    return x;
}

template<cppcon::regular T, cppcon::relation<T> R>
T& min(T& x, T& y, R r) {
    if (r(y, x)) {
        return y;
    }
    return x;
}

template<cppcon::totally_ordered T>
const T& max(const T& x, const T& y) {
    if (y < x) {
        return x;
    }
    return y;
}

template<cppcon::totally_ordered T>
T& max(T& x, T& y) {
    if (y < x) {
        return x;
    }
    return y;
}

template<cppcon::regular T, cppcon::relation<T> R>
const T& max(const T& x, const T& y, R r) {
    if (r(y, x)) {
        return x;
    }
    return y;
}

template<cppcon::regular T, cppcon::relation<T> R>
T& max(T& x, T& y, R r) {
    if (r(y, x)) {
        return x;
    }
    return y;
}

template<cppcon::forward_iterator It>
requires(regular<ValueType(It)>)
It unique(It first, It last) {
    if (first == last) { return last; }
    It result = first;
    ++first;
    while (first != last) {
        if (*result == *first) {
           ++first;
        } else {
            ++result;
            *result = *first;
            ++first;
        }
    }
    ++result;
    return result;
}

template<cppcon::forward_iterator It, cppcon::relation<ValueType(It)> R>
It unique(It first, It last, R r) {
    if (first == last) { return last; }
    It result = first;
    ++first;
    while (first != last) {
        if (*result == *first) {
           ++first;
        } else {
            ++result;
            *result = *first;
            ++first;
        }
    }
    ++result;
    return result;
}

}  // namespace cppcon


int main() {
    int x = 3;
    int y = 3;
    assert(&cppcon::min(x, y) == &x);
    assert(&cppcon::max(x, y) == &y);
    std::cout << cppcon::min(3, 4) << std::endl;
    std::cout << cppcon::min(3, 4, std::less<int>()) << std::endl;
    std::vector<int> vals;
    std::cout << cppcon::iterator<int*> << std::endl;
    std::cout << cppcon::forward_iterator<int*> << std::endl;
    std::cout << cppcon::input_iterator<int*> << std::endl;
    cppcon::unique(vals.begin(), vals.end());

    std::vector<std::unique_ptr<int>> vals2;
}
