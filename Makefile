LDLIBS=-lstdc++
## Enable c++0x/c++11 features. Dilute to taste.
#CPPFLAGS=-std=c++0x -DVMAP_CONFIG_NOEXCEPT -DVMAP_CONFIG_CBEGIN

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
