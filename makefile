.PHONY: run clean

EXE_NAME = exe
OBJ = src/Main.cpp
DEPENDENCIES = src/AST.hpp src/Utilities.hpp src/SymbolTable.hpp
FLAGS = -O0 -g -std=c++17 -Wall

build/${EXE_NAME}: ${OBJ} ${DEPENDENCIES} src/Scanner/lex.cc src/Parser/parse.cc
	g++ ${FLAGS} -o build/${EXE_NAME} ${OBJ} src/Scanner/lex.cc src/Parser/parse.cc

src/Scanner/lex.cc: src/Scanner/Scanner.l 
	flexc++ src/Scanner/Scanner.l --target-directory=src/Scanner

src/Parser/parse.cc: src/Parser/Parser.y
	bisonc++ src/Parser/Parser.y --target-directory=src/Parser

run: build/input.txt
	build/${EXE_NAME} <build/input.txt >build/output.txt 2>&1

clean:
	rm -rf build/${EXE_NAME} src/Scanner/lex.cc src/Scanner/Scannerbase.h src/Parser/parse.cc src/Parser/Parserbase.h