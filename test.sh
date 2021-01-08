#!/bin/bash

build/exe <build/input.txt >build/output.txt
llvm-as build/output.txt
lli build/output.txt.bc