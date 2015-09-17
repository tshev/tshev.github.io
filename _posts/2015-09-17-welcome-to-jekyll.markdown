---
layout: post
title:  "Generating uniform distribution from random number"
date:   2015-09-17 20:01:21
categories: jekyll update
---

Let's create file `generator.py`

{% highlight python %}


class Generator(object):
    def __init__(self, function, w0, *args):
        self.args = args
        self.storage = [w0]
        self.function = function

    def reserve(self, n):
        _ = self[n]

    def __getitem__(self, n):
        if type(n) == slice:
            self.reserve(n.stop)
            return self.storage[n]
        if n <= len(self.storage):
            return self.storage[n]
        minimum = len(self.storage)
        for i in range(minimum, n + 1):
            self.storage.append(self.function(self.storage[i - 1], *self.args))
        return self.storage[n]



{% endhighlight  %}


[tshev]:      http://tshev.org
[tshev-gh]:   https://github.com/tshev
[jekyll-help]: https://github.com/tshev
