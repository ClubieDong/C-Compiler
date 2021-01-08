#!/bin/bash

echo "Generating LLVM IR code..."          && \
build/exe tests/$1/test.cc                 && \

echo "Converting IR code to byte code..."  && \
llvm-as tests/$1/test.cc.ir                && \

echo "Running byte code..."                && \
lli tests/$1/test.cc.ir.bc
