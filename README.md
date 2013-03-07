VectorisedMap
=============

Simple implementation of a read-only vector-based map C++ 

Quite often, I find myself building std::map instances, which I then later on use purely as lookup tables. (Occasionally updating the mapped_type, but not adding or removing entries)

map gives pretty good worst-case lookup performance (OlogN) - and, equally importantly, provides a garunteed ordering of the contents. The downside is that it often has poor cache locality, since it's node-based and those nodes are scattered all over dynamically allocated memory.

What we have here, then, is an implementation which provides all of the O(logN) read-only lookup functions of map (find, upper_bound, etc), and preserves the ordering of the entries - but with everything stored in a single contiguous array. Thus, it's a 'faster' O(logN) than map (and also is slightly more memory efficient, since it's innards consist of a single allocation).

As a bonus, the iterators are now random-access iterators, rather than just bidirectional iterators, so some algorithms can use more optimal implementations.

Consider this a work-in-progress - it works, but still has a few rough edges - most notably the fact that the mapped_values are (effecitvely) read-only. This is due to the use of std::vector as the underlying implementation.  std::map::value_type is pair<const key,value> - but you can't put those in a vector, so we have to make do with what is effectively const pair<key,value>. There's a failry straightforward solution to this, which is not to use vector as the underlying implementation! (vector's overkill anyway - what we need here is actually a non-resizeable array, which can cope with having non-assignable elements; it also needs good swap/move performance. That's a relatively straightforward class to implement, but I had std::vector kicking around, and so haven't got around to it yet ;)


[ ] TODO: Find a good markdown-mode for The One True Editor!!!
