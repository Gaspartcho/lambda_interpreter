# Lambda Interpreter

This is the official repository for the **Lambda Interpreter**, an open source implementation of an interpreter for the Lambda-Prog (\lprog) programming language in C.

The specification for the \lprog language can be found in the file [docs/language_spec.md](docs/language_spec.md).

## Building

Requirements: C compiler, CMake

```sh
cd lambda_interpreter
mkdir build && cd build
cmake ..
make
```
