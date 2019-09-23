#include <iostream>
#include <numeric>
#include <algorithm>
#include <memory>
#include <vector>
#include <cassert>
#include <type_traits>
#include <cppcon/cppcon.h>

// TODO: properties for relations

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


template<typename T, unary_function<T> Projection>
requires totally_ordered<codomain_t<Projection, T>>
const T& min(const T& x, const T& y, Projection projection) {
    if (projection(y) < projection(x)) {
        return y;
    }
    return x;
}


template<typename T, unary_function<T> Projection>
requires totally_ordered<codomain_t<Projection, T>>
T& min(T& x, T& y, Projection projection) {
    if (projection(y) < projection(x)) {
        return y;
    }
    return x;
}

template<typename T, weak_strict_ordering<T> R>
const T& min(const T& x, const T& y, R r) {
    if (r(y, x)) {
        return y;
    }
    return x;
}

template<typename T, weak_strict_ordering<T> R>
T& min(T& x, T& y, R r) {
    if (r(y, x)) {
        return y;
    }
    return x;
}


template<totally_ordered T>
const T& max(const T& x, const T& y) {
    if (y < x) {
        return x;
    }
    return y;
}


template<totally_ordered T>
T& max(T& x, T& y) {
    if (y < x) {
        return x;
    }
    return y;
}

template<regular T, relation<T> R>
const T& max(const T& x, const T& y, R r) {
    if (r(y, x)) {
        return x;
    }
    return y;
}



template<regular T, relation<T> R>
T& max(T& x, T& y, R r) {
    if (r(y, x)) {
        return x;
    }
    return y;
}

template<typename T, unary_function<T> Projection>
requires totally_ordered<codomain_t<Projection, T>>
const T& max(const T& x, const T& y, Projection projection) {
    if (projection(y) < projection(x)) {
        return x;
    }
    return y;
}


template<typename T, unary_function<T> Projection>
requires totally_ordered<codomain_t<Projection, T>>
T& max(T& x, T& y, Projection projection) {
    if (projection(y) < projection(x)) {
        return x;
    }
    return y;
}

template<forward_iterator It, output_iterator Out, relation<value_type_t<It>> R>
requires readable<It> && writable<Out, value_type_t<It>>
Out unique_copy(It first, It last, Out out, R r) {
    if (first == last) { return out; }
    *out = *first;  
    ++out;
    It previous = first; ++first;
    while (first != last) {
        if (!r(*previous, *first)) {
          *out = *first;
          ++out;
        }
        ++first;
        ++previous;
    }
    return out;
}

template<forward_iterator It, output_iterator Out>
requires readable<It> && writable<Out, value_type_t<It>>
Out unique_copy(It first, It last, Out out) {
    return cppcon::unique_copy(first, last, out, std::equal_to<value_type_t<It>>());
}

template<forward_iterator It, relation<value_type_t<It>> R>
requires readable<It> && writable<It>
It unique(It first, It last, R r) {
    if (first == last) { return last; }
    It result = first; ++first;
    while (first != last) {
        if (!r(*result, *first)) {
            ++result;
            *result = *first;
        }
        ++first;
    }
    ++result;
    return result;
}


template<forward_iterator It>
requires(regular<value_type_t<It>> && readable<It> && writable<It>)
It unique(It first, It last) {
    return cppcon::unique(first, last, std::equal_to<value_type_t<It>>());
}


template<forward_iterator It, additive_monoid N, relation<value_type_t<It>> R>
requires readable<It>
N unique_count(It first, It last, N n, R r) {
    if (first == last) { return n; }
    It previous = first;
    ++first;
    while (first != last) {
        if (r(*previous, *first)) {
           ++previous;
           ++first;
        } else {
            ++first;
            ++previous;
            ++n;
        }
    }
    ++n;
    return n;
}


template<forward_iterator It, additive_monoid N>
requires(readable<It>)
N unique_count(It first, It last, N n) {
    return unique_count(first, last, n, std::equal_to<value_type_t<It>>());
}


template<forward_iterator It, additive_monoid N>
requires readable<It> && regular<value_type_t<It>>
std::pair<It, N> find_not(It first, It last, const value_type_t<It>& x, N n) {
    while (first != last && *first == x) {
        ++first;
        ++n;
    }
    return {first, n};
}


template<forward_iterator It, relation< value_type_t<It> > R, additive_monoid N>
requires(readable<It>)
std::pair<It, N> find_if_not(It first, It last, const value_type_t<It>& x, R r, N n) {
    while (first != last && r(*first, x)) {
        ++first;
        ++n;
    }
    return {first, n};
}


template<forward_iterator It, output_iterator Out>
requires readable<It> && writable<Out, std::pair<value_type_t<It>, size_t>>
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

template<forward_iterator It, output_iterator Out, relation<value_type_t<It>> R>
requires readable<It> && writable<Out, std::pair<value_type_t<It>, size_t>>
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


template<forward_iterator It, output_iterator Out0, output_iterator Out1>
requires readable<It> && writable<Out0, value_type_t<It>> && writable<Out1, size_t>
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

template<forward_iterator It, output_iterator Out0, forward_iterator Out1>
requires readable<It> && writable<Out0> && writable<Out1>
std::pair<Out0, Out1> frequencies(It f, It l, Out0 out0, Out1 out1) {
  typedef value_type_t<Out1> N;

  while (f != l) { 
    It it = f;
    N n = 1;
    ++f;
    It r = find_not(f, l, *it, n);

    *out0 = *it;
    ++out0;

    *out1 = r.second;
    ++out1;

    f = r.first;
  } 
  return {out0, out1};
}


template<forward_iterator It,
         typename Out,
         relation<value_type_t<It>> P,
         functional_procedure<It, It> F>
requires readable<It> && (forward_iterator<Out> || output_iterator<Out>)  && writable<Out, codomain_t<F, It, It>> 
Out transform_subgroups(It first, It last, Out out, P pred, F function) {
    if (first == last) { return out; }
    It previous = first;
    It fast = previous;
    ++fast;
    while (fast != last) {
        if (!pred(*previous, *fast)) {
            *out = function(first, fast);
            ++out;
            first = fast;
        }
        ++previous; ++fast;
    }
    *out = function(first, fast);
    return ++out;
}


template<forward_iterator It,
         typename Out,
         relation<value_type_t<It>> P,
         functional_procedure<It, distance_type_t<It>> F>
requires readable<It> && (forward_iterator<Out> || output_iterator<Out>) && writable<Out, codomain_t<F, It, distance_type_t<It>> >
Out transform_subgroups(It first, It last, Out out, P pred, F function) {
    if (first == last) { return out; }
    It previous = first;
    It fast = previous;
    ++fast;
    distance_type_t<It> n = 0;
    while (fast != last) {
        if (!pred(*previous, *fast)) {
            *out = function(first, n);
            n = 0;
            ++out;
            first = fast;
        }
        ++n;
        ++previous; ++fast;
    }
    *out = function(first, n);
    return ++out;
}


template<forward_iterator It,
         relation<value_type_t<It>> Predicate,
         functional_procedure<It, It> F>
requires readable<It> && writable<It, codomain_t<F, It, It>>
It squash_subgroups(It first, It last, Predicate pred, F function) {
    return transform_subgroups(first, last, first, pred, function);
}

template<forward_iterator It, functional_procedure<It, It> F>
requires readable<It>
void split(It first, It last, const value_type_t<It>& value, F f) {
    while (first != last) {
        It middle = std::find(first, last, value);
        f(first, middle);
        first = middle;
        if (first != last) {
            ++first;
        }
    }
}

template<forward_iterator It, output_iterator Out, functional_procedure<It, It> F>
requires writable<Out, codomain_t<F, It, It>>
Out transform_split(It first, It last, Out out, const value_type_t<It>& value, F f) {
    while (first != last) {
        It middle = std::find(first, last, value);
        *out = f(first, middle);
        ++out;
        first = middle;
        if (first != last) {
            ++first;
        }
    }
    return out;
}


template<forward_iterator It0, forward_iterator It1>
requires readable<It0> && readable<It1>
std::pair<It0, It1> find_missmatch(It0 first0, It0 last0, It1 first1, It1 last1) {
    while (first0 != last0 && first1 != last1 && *first0 == *first1) {
        ++first0;
        ++first1;
    }
    return {first0, first1};
}


template<random_access_iterator It0, random_access_iterator It1>
requires readable<It0> && readable<It1>
std::pair<It0, It1> find_missmatch(It0 first0, It0 last0, It1 first1, It1 last1) {
    if (last0 - first0 < last1 - first1) {
        while (first1 != last1 && *first0 == *first1) {
            ++first0; ++first1;
        }
    } else {
        while (first0 != last0 && *first0 == *first1) {
            ++first0; ++first1;
        }
    }
    return {first0, first1};
}


template<forward_iterator It0, forward_iterator It1>
requires readable<It0> && readable<It1>
It0 find_subrange(It0 first0, It0 last0, It1 first1, It1 last1) {
    while (first0 != last0 &&
           find_missmatch(first0, last0, first1, last1).second != last1) {
        ++first0;
    }
    return first0;
}


template<forward_iterator It0, forward_iterator It1, functional_procedure<It0, It0> F>
requires readable<It0> && readable<It1>
void split(It0 first0, It0 last0, It1 first1, It1 last1, F f) {
    while (first0 != last0) {
        It0 middle = std::search(first0, last0, first1, last1);
        f(first0, middle);
        first0 = middle;
        if (first0 != last0) {
            ++first0;
        }
    }
}


template<forward_iterator It0, output_iterator Out, forward_iterator It1, functional_procedure<It0, It0> F>
requires readable<It0> && readable<It1> && writable<Out, codomain_t<F, It0, It0>>
Out transform_split(It0 first0, It0 last0, Out out, It1 first1, It1 last1, F f) {
    auto step_size = std::distance(first1, last1);
    while (first0 != last0) {
        It0 middle = std::search(first0, last0, first1, last1);
        *out = f(first0, middle);
        ++out;
        first0 = middle;
        if (first0 != last0) {
            std::advance(first0, step_size);
        }
    }
    return out;
}


template<forward_iterator It, unary_predicate<value_type_t<It>> P>
requires readable<It> && writable<It>
It remove(It first, It last, const value_type_t<It>& value) {
    first = std::find(first, last, value);
    if (first == last) { return last; }
    It fast = first; ++fast;
    while (fast != last) {
        if (*fast == value) {
            ++fast;
        } else {
            *first = std::move(*fast);
            ++first; ++fast;
        }
    }
    return first;
}

template<forward_iterator It, unary_predicate<value_type_t<It>> P>
requires readable<It> && writable<It>
It remove_if(It first, It last, P pred) {
    first = std::find_if(first, last, pred);
    if (first == last) { return last; }
    It fast = first; ++fast;
    while (fast != last) {
        if (pred(*fast)) {
            ++fast;
        } else {
            *first = std::move(*fast);
            ++first; ++fast;
        }
    }
    return first;
}

template<forward_iterator It, unary_predicate<value_type_t<It>> P>
requires readable<It> && writable<It>
It semistable_partition(It first, It last, P pred) {
    first = std::find_if(first, last, pred);
    if (first == last) { return last; }
    It fast = first; ++fast;
    while (fast != last) {
        if (pred(*fast)) {
            ++fast;
        } else {
            std::swap(*first, *fast);
            ++first; ++fast;
        }
    }
    return first;
}


template<forward_iterator It0, forward_iterator It1, binary_predicate<value_type_t<It0>, value_type_t<It1>> P>
requires readable<It0> && readable<It1>
std::pair<It0, It1> find_if(It0 first0, It0 last0, It1 first1, P pred) {
    while (first0 != last0 && !pred(*first0, first1)) {
        ++first0;
        ++first1;
    }
    return {first0, first1};
}


template<forward_iterator It0, forward_iterator It1, binary_predicate<value_type_t<It0>, value_type_t<It1>> P>
requires (readable<It0> && writable<It0>) && (readable<It1> && writable<It1>)
std::pair<It0, It1> semistable_partition(It0 first0, It0 last0, It1 first1, P pred) {
    std::pair<It0, It1> r = find_if(first0, last0, first1, pred);
    first0 = r.first; first1 = r.second;
    if (first0 == last0) { return {first0, first1}; }
    It0 fast0 = first0; ++fast0;
    It1 fast1 = first1; ++fast1;
    while (fast0 != last0) {
        if (pred(*fast0, *fast1)) {
            ++fast0; ++fast1;
        } else {
            std::swap(*first0, *fast0);
            ++first0; ++fast0;
            std::swap(*first1, *fast1);
            ++first1; ++fast1;
        }
    }
    return {first0, first1};
}


}  // namespace cppcon

void journey0() {
  int x = 3;
  int y = 3;

  assert(&cppcon::min(x, y) == &x);
  assert(&cppcon::max(x, y) == &y);

  assert(&cppcon::min(x, y, std::less<int>()) == &x);
  assert(&cppcon::max(x, y, std::less<int>()) == &y);

  std::pair<int, int> px(1, 4);
  std::pair<int, int> py(1, 3);

  assert(&cppcon::min(px, py, [](auto x) { return x.first; }) == &px);
  assert(&cppcon::max(px, py, [](auto x) { return x.first; }) == &py);

  std::vector<int> vals;
  cppcon::unique(vals.begin(), vals.end());
}

void journey1() {
    std::vector<int> values = {1, 1, 2, 3, 3, 3, 4, 4, 5};
    auto values_copy = values;
    std::vector<int> unique_copies;
    cppcon::unique_copy(std::begin(values), std::end(values), std::back_inserter(unique_copies));

    
    assert(cppcon::unique(std::begin(values_copy), std::end(values_copy)) - std::begin(values_copy) == 5);
    assert(cppcon::unique_count(std::begin(values), std::end(values), 0) == 5);

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
    assert(unique_copies == expected_unique_elements);
}

void journey2() {
    std::vector<int> values = {1, 1, 2, 3, 3, 3, 4, 4, 5};
    std::vector<std::pair<int, size_t>> values_and_frequencies;
    cppcon::transform_subgroups(
        std::begin(values),
        std::end(values),
        std::back_inserter(values_and_frequencies),
        std::equal_to<int>(),
        [](auto first, size_t n) { return std::pair<int, size_t>(*first, n); }
    );

    std::vector<std::pair<char, int>> pairs = {{'a', 10}, {'a', 2}, {'b', 10}, {'c', 12}};
    std::vector<std::pair<char, int>> expected_pairs = {{'a', 12}, {'b', 10}, {'c', 12}};

    
    struct equal_first {
        bool operator()(std::pair<char, int> x, std::pair<char, int> y) const {
            return x.first == y.first;
        }
    };
    typedef decltype(pairs)::iterator iterator;
    auto middle = cppcon::squash_subgroups(std::begin(pairs), std::end(pairs), equal_first(), [](iterator first, iterator last) {
        int n = std::accumulate(first, last, 0, [](int x, auto y) { return x + y.second; });
        return std::pair<char, int>(first->first, n);
    });
}


void journey3() {
    std::string str = "a_b_c_de_";
    std::string str_new;
    cppcon::split(std::begin(str), std::end(str), '_', [&str_new](auto first, auto last) { std::copy(first, last, std::back_inserter(str_new)); });
    assert(str_new == "abcde");
    std::string str_out;

    cppcon::transform_split(std::begin(str), std::end(str), std::back_inserter(str_out), '_', [](auto first, auto last) { return *first; });
    assert(str_out == "abcd");

    str_out.clear();
    str = "a__b__c__d";
    std::string delimiter = "__";
    cppcon::transform_split(std::begin(str), std::end(str), std::back_inserter(str_out), std::begin(delimiter), std::end(delimiter), [](auto first, auto last) { return *first; });
    assert(str_out == "abcd");
}

void journey4() {
  struct is_even {
    bool operator()(int x) const { return x % 2 == 0; }
  };
  std::vector<int> values = {1, 2, 3, 4, 5, 6};
  std::vector<int> expected_values = {1, 3, 5};

  auto middle = cppcon::remove_if(values.begin(), values.end(), is_even());
  values.resize(middle - values.begin());
  assert(values == expected_values);

  values = {1, 2, 3, 4, 5, 6};
  expected_values = {1, 3, 5, 4, 2, 6};
  middle = cppcon::semistable_partition(values.begin(), values.end(), is_even());
  assert(std::equal(std::begin(values), middle, values.begin(), values.begin() + 3));
  assert(values == expected_values);
}

int main() {
    journey0(); // min, max
    journey1(); // unique, unique_copy
    journey2(); // frequencies
    journey3(); // split
    journey4(); // remove_if, semistable_partition


    struct foo {
        bool operator()(int x, int y) {
            return true;
        }
    };
}
