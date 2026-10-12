CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2

# Build the CLI (prints the tree to stdout).
tree: main.c
	$(CC) $(CFLAGS) -o tree main.c

# Build the test (reuses the implementation, so exclude the CLI main).
test: main.c main_test.c
	$(CC) $(CFLAGS) -DTREE_NO_MAIN -o test main.c main_test.c

# Build and run the tests.
check: test
	./test

clean:
	rm -f tree test

.PHONY: tree test check clean
