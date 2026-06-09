# Code examples

There are several well-known code examples in different programming languages.

### Tests

Some of them are covered by the test scripts.
The tests are a pure shell scripts.
Those are based on the `expect` tool.
They provide some input and wait for appropriate output.
The return value is 0, if the test pass.

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

Rust (via rust-script):
```bash
./test.exp rust-script 00-hello-world.rs
```
