#!/bin/bash

rm bin/native/native-ltc4150.o
rm theone.a
ar -rc theone.a bin/native/*.o
g++ -g -std=c++11 -static-libstdc++ -static-libgcc main.cpp -o wtf.elf -Wl,--whole-archive theone.a -Wl,--no-whole-archive -Wl,--no-as-needed -ldl
