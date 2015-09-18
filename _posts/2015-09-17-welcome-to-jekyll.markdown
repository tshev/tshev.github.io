---
layout: post
title:  "Generating uniform distribution from a random number"
date:   2015-09-17 20:01:21
categories: python
---

![uniform distrbution](/images/post1/post1.png)

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

Let's write `visualize.py`


{% highlight python %}
#!/usr/bin/env python
import contextlib
import matplotlib
import numpy as np
import sys

if sys.version_info[0] < 3:
    import Tkinter as Tk
else:
    import tkinter as Tk

matplotlib.use('TkAgg')

from numpy import arange, sin, pi
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, \
                                       NavigationToolbar2TkAgg
from generator import Generator
from matplotlib.figure import Figure


def recurrence1(before, a, b, c):
    return np.modf((before * a + b)/c)[0]
    
def create_figure(dpi):
    f = Figure(figsize=(6, 6), dpi=dpi)
    a = f.add_subplot(111)
    return f, a



class Application(object):
    def __init__(self, title):
        parameter = np.random.randint(1, 100)
        self.generator = Generator(recurrence1, parameter, 10011, 23, 13)
        self.generator.reserve(10**6)
        self.root = Tk.Tk()
        self.figure, self.plot = create_figure(100)
        self.inp = Tk.Entry(self.root, bd = 5)
        self.label_n = Tk.Label(self.root, text="N:")
        self.button = Tk.Button(master=self.root, text='Quit',
                                command=self._quit)
        self.canvas = FigureCanvasTkAgg(self.figure, master=self.root)
        self.root.wm_title(title)
        self.calculate = Tk.Button(master=self.root, text='Calculate',
                                   command=self._plot_hist)
        self.toolbar = NavigationToolbar2TkAgg(self.canvas, self.root )

    def _plot_hist(self):
        try:
            n = int(self.inp.get())
            self.figure.delaxes(self.plot)
            self.plot = self.figure.add_subplot(111)
            self.plot.hist(self.generator[1:n + 1], bins=5)
            self.canvas.draw()
            print(n)
        except Exception as e:
            print e

    def pack(self):
        self.canvas._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)
        self.label_n.pack(side=Tk.LEFT)
        self.inp.pack(side=Tk.LEFT)
        self.calculate.pack(side=Tk.LEFT)
        self.button.pack(side=Tk.RIGHT)
        self.toolbar.update()

    def _quit(self):
        self.root.quit()
        self.root.destroy()  
    
    def close(self):
        Tk.mainloop()

def main(argv):
    with contextlib.closing(Application("Lab #1")) as app:
        app.pack()


{% endhighlight %}

[tshev]:      http://tshev.org
[tshev-gh]:   https://github.com/tshev
[jekyll-help]: https://github.com/tshev
