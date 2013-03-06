LDLIBS=-lstdc++
#CPPFLAGS=-std=c++0x

.PHONY: all
all: test

.PHONY: clean
clean:
	-rm vmap-test vmap-test.o

.PHONY: test
test: vmap-test
	./vmap-test

vmap-test: vmap-test.o

vmap-test.o : vmap-test.cpp vmap.h
