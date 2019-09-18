#pragma once
namespace cppcon {
#define ValueType(T) typename std::iterator_traits<T>::value_type

template<typename T>
using value_type_t = typename std::iterator_traits<T>::value_type;

#define DifferenceType(T) typename std::iterator_traits<T>::difference_type

template<typename T>
using difference_type_t = typename std::iterator_traits<T>::difference_type;

#define DistanceType(T) typename std::make_unsigned<DifferenceType(T)>::type

template<typename T>
using distance_type_t = typename std::make_unsigned<DifferenceType(T)>::type;

#define Codomain(F, U...) typename std::invoke_result<F, U>::type

template<typename F, typename... U>
using codomain_t = typename std::invoke_result<F, U...>::type;

template<typename T, typename = void>
struct is_equality_comparable : std::false_type {};

template<typename T>
struct is_equality_comparable<T,
    typename std::enable_if<std::is_same<decltype(std::declval<T&>() == std::declval<T&>()), bool>::value>::type
    > : std::true_type
{
};


template<typename T, typename = void>
struct is_less_than_comprable: std::false_type {};

template<typename T>
struct is_less_than_comprable<T, typename std::enable_if<std::is_same<decltype(std::declval<T&>() < std::declval<T&>()), bool>::value>::type> : std::true_type {};

template<typename T, typename U = ValueType(T)> concept writable = requires(T it, U x) { *it = x; }; 
//template<typename T> concept writable = true; 


template<typename T>
concept readable = std::is_same<decltype(*std::declval<T>()), value_type_t<T>&>::value || std::is_same<decltype(*std::declval<T>()), const value_type_t<T>&>::value;;

template<typename T>
concept semiregular = std::is_default_constructible<T>::value && std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value;

template<typename T>
concept regular = semiregular<T> && cppcon::is_equality_comparable<T>::value;

template<typename T>
concept totally_ordered = regular<T> && is_less_than_comprable<T>::value;

template<typename R, typename T>
concept relation = std::is_same<typename std::invoke_result<R, T, T>::type, bool>::value;

template<typename R, typename T>
concept weak_strict_ordering = regular<T> && relation<R, T>; 

template<typename R, typename T>
concept weak_strict_ordering = regular<T> && relation<R, T>;

template<typename F, typename... T>
concept functional_procedure = regular<typename std::invoke_result<F, T...>::type> || std::is_same<typename std::invoke_result<F, T...>::type, void>::value;


template<typename F, typename... T>
concept predicate = functional_procedure<F, T...> && std::is_same<typename std::invoke_result<F, T...>::type, bool>::value;

template<typename P, typename T>
concept unary_predicate = predicate<P, T>;

template<typename P, typename T0, typename T1>
concept binary_predicate = predicate<P, T0, T1>;

template<typename I>
concept iterator = std::is_same<std::forward_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value ||
                   std::is_same<std::input_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value ||
                   std::is_same<std::output_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value ||
                   std::is_same<std::bidirectional_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value ||
                   std::is_same<std::random_access_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename I>
concept forward_iterator = iterator<I> && std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename I>
concept input_iterator = iterator<I> && std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename I>
concept bidirectional_iterator = iterator<I> && std::is_base_of<std::bidirectional_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename I>
concept output_iterator = iterator<I> && std::is_base_of<std::output_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename I>
concept random_access_iterator= iterator<I> && std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<I>::iterator_category>::value;

template<typename T>
concept additive_semigroup = regular<T> && std::is_same<decltype(T() + T()), T>::value; // requires(T x, T y) { T(x + y); } TODO: think about implicit convertions

template<typename T>
concept additive_monoid = additive_semigroup<T>; // 0 \in T, identity_element(T);
} // namespace cppcon
