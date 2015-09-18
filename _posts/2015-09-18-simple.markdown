---
layout: post
title:  "Alternative for sizeof(array)/sizeof(array[0])"
date:   2015-09-18 15:29:21
categories: c++
---


How to find sizeof of stack based array
Chrome way

{% highlight cpp %}
template <typename T, size_t N> 
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
{% endhighlight %}

My alternative
{% highlight cpp %}

template<typename T, size_t N>
inline size_t array_size(T (&array)[N]) {
    return N;
}

{% endhighlight %}
[tshev]:      http://tshev.org
[tshev-gh]:   https://github.com/tshev
[jekyll-help]: https://github.com/tshev
