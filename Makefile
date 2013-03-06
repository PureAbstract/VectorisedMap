LDLIBS=-lstdc++
#CPPFLAGS=-std=c++0x

.PHONY: all
all: vmap test

.PHONY: clean
clean:
	-rm vmap vmap.o

.PHONY: test
test: vmap
	./vmap

vmap: vmap.o

vmap.o : vmap.cpp vmap.h
