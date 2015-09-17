---
layout: post
title:  "Herbs C++ ABI proposal"
date:   2015-09-17 20:01:21
categories: python
---
A  C++  developer  cannot  compile  C++  code  and  share  the  object  file  with  other  C++  developers  on  the 
same platform and know that the result will 
compile and link correctly
.
Our status quo is that two source files a.cpp and b.cpp can only be linked together if they are compiled with both:

* the same version of the same compiler, or another compiler with a compatibility mode; and
* compatible switch settings, since most C++ compilers offer incompatible switch settings where even compiling two files with the same version of the same compiler will not link successfully

More details: <https://isocpp.org/files/papers/n4028.pdf>

[tshev]:      http://tshev.org
[tshev-gh]:   https://github.com/tshev
[jekyll-help]: https://github.com/tshev
