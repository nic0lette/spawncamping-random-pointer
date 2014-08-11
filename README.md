spawncamping-random-pointer
===========================

What happens if you make a pointer from random() and dereference it?

Of course the answer is that you get a SIGSEGV.
... Well, usually you do. The question is, what's the likelihood of that? Could you ever do it and not have it segfault?

Now you can find out!

Building
--------

It should be pretty easy to build as it doesn't use any external libraries, and *should* be portable... Or, at least as portable as a segfault generator is.

I'm not sure it will work in Windows. I didn't put in any crazy definition statements, so probably not ;)

A final note
------------

This program was inspired by Dr. Robert Harper's comment, but it isn't my intention to mock (or really anything) him, especially since he specifically says that this won't work, but it was still his comment that led to this, so I "credited" him with it.
