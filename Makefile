CC = gcc
CFLAGS = -Wall -Wextra -Iunity/src

all: build/db build/tests

build/db: src/main.c
	mkdir -p build
	$(CC) $(CFLAGS) -o build/db src/main.c

build/tests: tests/test.c unity/src/unity.c
	mkdir -p build
	$(CC) $(CFLAGS) -o build/tests tests/test.c unity/src/unity.c

clean:
	rm -rf build

.PHONY: all clean
