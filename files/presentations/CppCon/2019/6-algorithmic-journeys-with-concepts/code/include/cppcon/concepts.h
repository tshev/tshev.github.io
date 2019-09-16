#pragma once
namespace cppcon {

template<typename T, typename = void>
struct is_equality_comparable : std::false_type
{ };

template<typename T>
struct is_equality_comparable<T,
    typename std::enable_if<std::is_same<decltype(std::declval<T&>() == std::declval<T&>()), bool>::value>::type
    > : std::true_type
{
};


template<typename T, typename = void>
struct is_less_than_comprable: std::false_type {};

template<typename T>
struct is_less_than_comprable<T,
    typename std::enable_if<std::is_same<decltype(std::declval<T&>() < std::declval<T&>()), bool>::value>::type
    > : std::true_type
{
};

template<typename T, typename = void>
struct has_plus: std::false_type {};

template<typename T>
struct has_plus<T,
    typename std::enable_if<std::is_same<decltype(std::declval<T&>() + std::declval<T&>()), T>::value>::type
    > : std::true_type
{
};



template<typename T>
concept semiregular = std::is_default_constructible<T>::value && std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value;

template<typename T>
concept regular = semiregular<T> && cppcon::is_equality_comparable<T>::value;

template<typename T>
concept totally_ordered = regular<T> && cppcon::is_less_than_comprable<T>::value;

template<typename T, typename U>
concept relation = std::is_same<typename std::invoke_result<T, U, U>::type, bool>::value;

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

#define ValueType(T) typename std::iterator_traits<T>::value_type

template<typename T>
concept additive_semigroup = regular<T> && has_plus<T>::value;

template<typename T>
concept additive_monoid = additive_semigroup<T>; // 0 \in T, identity_element(T);

} // namespace cppcon
