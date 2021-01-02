#!/bin/bash
echo "Running flex..." && \
flexc++ src/Scanner/Scanner.l --target-directory=src/Scanner && \
echo "Running bison..." && \
bisonc++ src/Parser/Parser.y --target-directory=src/Parser && \
cat src/Parser/Hack >> src/Parser/parse.cc  && \
echo "Compiling..." && \
g++ -O0 -g -std=c++17 -Wall -o build/exe src/Main.cpp src/Scanner/lex.cc src/Parser/parse.cc src/AST/AST.cpp&& \
echo "Done!"
