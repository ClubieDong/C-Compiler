# C-Compiler

> An LLVM based C-like language compiler

## Build

To build this repo, you need:
1. LLVM 10.0.0
2. Flexc++ 2.07.07
3. Bisonc++ 6.03.00

Use commands below to install:
```bash
sudo apt install llvm-dev
sudo apt install flexc++
sudo apt install bisonc++
```

Use commands below to build:
```bash
git clone https://github.com/ClubieDong/C-Compiler.git
cd C-Compiler
./build.sh
```

## Run

The generated executable file accepts only one argument, the C-language file:
```bash
build/exe <file>
```

It outputs:
* \<file\>.lex, which contains the result of scanner
* \<file\>.ast, which contains the Abstract Syntax Tree(AST)
* \<file\>.ir, which contains the generated LLVM IR code

## Test

Directory `tests` contains all test cases, use `test.sh` to run one of those, for example:
```bash
./test.sh Basic/Test1
```

## License

The project is licensed under MIT license.

## Contact

Email: 810443053@qq.com
