#include <iostream>
#include <algorithm>
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


template<cppcon::forward_iterator It, cppcon::additive_monoid N, cppcon::relation<ValueType(It)> R>
N unique_count(It first, It last, N n, R r) {
    if (first == last) { return n; }
    It result = first;
    ++first;
    while (first != last) {
        if (*result == *first) {
           ++first;
        } else {
            ++n;
            ++first;
            ++result;
        }
    }
    ++n;
    return n;
}


template<cppcon::forward_iterator It, cppcon::additive_monoid N>
N unique_count(It first, It last, N n) {
    return unique_count(first, last, n, [](const auto& x, const auto& y) { return x == y; });
}


template<cppcon::forward_iterator It, cppcon::additive_monoid N>
std::pair<It, N> find_not(It first, It last, const ValueType(It)& x, N n) {
    while (first != last && *first == x) {
        ++first;
        ++n;
    }
    return {first, n};
}


template<cppcon::forward_iterator It, cppcon::relation<ValueType(It)> R, cppcon::additive_monoid N>
std::pair<It, N> find_if_not(It first, It last, const ValueType(It)& x, R r, N n) {
    while (first != last && r(*first, x)) {
        ++first;
        ++n;
    }
    return {first, n};
}


template<cppcon::forward_iterator It, cppcon::output_iterator Out>
inline
Out frequencies(It f, It l, Out out) {
  typedef size_t N;
  while (f != l) { 
    It it = f;
    N n = 1;
    ++f;
    auto r = find_not(f, l, *it, n);
    f = r.first;
    *out = { *it, r.second };
    ++out;
  } 
  return out;
}

template<cppcon::forward_iterator It, cppcon::output_iterator Out, cppcon::relation<std::pair<ValueType(It), size_t>> R>
inline
Out frequencies(It f, It l, Out out, R r) {
  typedef size_t N;
  while (f != l) { 
    It start = f;
    N n = 1;
    ++f;
    auto r = find_if_not(f, l, *start, r, n);
    f = r.first;
    *out = { *start, r.second };
    ++out;
  } 
  return out;
}


template<cppcon::forward_iterator It, cppcon::output_iterator Out0, cppcon::output_iterator Out1>
inline
std::pair<Out0, Out1> frequencies(It f, It l, Out0 out0, Out1 out1) {
  typedef size_t N;

  while (f != l) { 
    It start = f;
    N n = 1;
    ++f;
    auto r = find_not(f, l, *start, n);

    *out0 = *start;
    ++out0;

    *out1 = r.second;
    ++out1;

    f = r.first;
  } 
  return {out0, out1};
}

template<cppcon::forward_iterator It, cppcon::output_iterator Out0, cppcon::forward_iterator Out1>
inline
std::pair<Out0, Out1> frequencies(It f, It l, Out0 out0, Out1 out1) {
  typedef ValueType(Out1) N;

  while (f != l) { 
    It it = f;
    N n = 1;
    ++f;
    auto r = find_not(f, l, *it, n);

    *out0 = *it;
    ++out0;

    *out1 = r.second;
    ++out1;

    f = r.first;
  } 
  return {out0, out1};
}




}  // namespace cppcon


int main() {
    int x = 3;
    int y = 3;
    assert(&cppcon::min(x, y) == &x);
    assert(&cppcon::max(x, y) == &y);
    std::vector<int> vals;
    cppcon::unique(vals.begin(), vals.end());

    std::vector<std::unique_ptr<int>> vals2;

    std::vector<int> values = {1, 1, 2, 3, 3, 3, 4, 4, 5};
    auto values_copy = values;
    assert(cppcon::unique(std::begin(values_copy), std::end(values_copy)) - std::begin(values_copy) == 5);
    std::cout << cppcon::unique_count(std::begin(values), std::end(values), 0)  << std::endl;

    std::vector<std::pair<int, size_t>> values_frequencies;
    cppcon::frequencies(std::begin(values), std::end(values), std::back_inserter(values_frequencies));

    std::vector<std::pair<int, size_t>> expected_frequencies = {{1, 2ul}, {2, 1ul}, {3, 3ul}, {4, 2ul}, {5, 1ul}};
    assert(values_frequencies == expected_frequencies);

    std::vector<int> unique_elements;
    std::vector<size_t> occurences;
    cppcon::frequencies(std::begin(values), std::end(values), std::back_inserter(unique_elements), std::back_inserter(occurences));
    std::vector<int> expected_unique_elements = {1, 2, 3, 4, 5};
    std::vector<size_t> expected_occurences= {2ul, 1ul, 3ul, 2ul, 1ul};
    assert(unique_elements == expected_unique_elements);
    assert(occurences == expected_occurences);


    std::cout << "frequencies\n";
    for (auto x : values_frequencies) std::cout << x.first << " " << x.second << std::endl;
}
