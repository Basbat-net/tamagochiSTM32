CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude

.PHONY: test clean

test: build/tamagotchi_test
	./build/tamagotchi_test

build/tamagotchi_test: src/tamagotchi.c tests/test_tamagotchi.c include/tamagotchi.h
	mkdir -p build
	$(CC) $(CFLAGS) src/tamagotchi.c tests/test_tamagotchi.c -lm -o $@

clean:
	rm -rf build
