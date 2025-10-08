# Lambda Interpreter

This is the official repository for the **Lambda Interpreter (\lprogi)**, an open source implementation of an interpreter for the Lambda-Prog (\lprog) programming language in C.

See [LICENSE](LICENSE) for information on how this software is licensed.

The specification for the \lprog language can be found in the file [docs/language_spec.md](docs/language_spec.md).


## Building

Requirements: C compiler, CMake

```sh
cd lambda_interpreter
mkdir build && cd build
cmake ..
make
```

## Usage

```sh
lprogi [options...] [files...]
```

### Options

- `-h` : Prints this help and exits
- `-i` : Runs in interactive mode after running the files
