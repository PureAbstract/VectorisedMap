LDLIBS=-lstdc++
#CPPFLAGS=-std=c++0x

.PHONY: all
all: vmap

.PHONY: clean
clean:
	-rm vmap vmap.o

vmap: vmap.o

vmap.o : vmap.cpp vmap.h
