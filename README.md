# Code examples

There are several well-known code examples in different programming languages.

### Tests

All of them are covered by the same test scripts.
Test scripts are pure shell scripts that use the `expect` tool to interact with the program.

### Examples

C:
```bash
gcc 00-hello-world.c
./test.exp ./a.out
```

Python:
```bash
./test.exp ./01-enter-number.py
```

Go:
```bash
./test.exp go run 01-enter-number.py
```

