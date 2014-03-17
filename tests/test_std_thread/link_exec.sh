#!/bin/bash

GOBACK=$PWD
cd shared
#rm libstartup.so
#rm libuart0.so

for i in lib*.so
do
    libs="$PWD/$i $libs"
done

#echo $libs
cd $GOBACK
NP="$PWD/bin/native"
# g++ -g -std=c++11 main.cpp -o wtf.elf $libs $NP/cpu.a $NP/native_base.a $NP/uart0.a $NP/posix.a $NP/core.a $NP/sys.a $NP/lib.a -Wl,--no-as-needed -ldl #-pthread

 g++ -g -c -std=c++11 main.cpp -o wtf.o -Wl,--no-as-needed -ldl -D_REENTRANT #-pthread
 g++ wtf.o -o wtf.elf -std=c++11 $libs $NP/cpu.a $NP/native_base.a $NP/uart0.a $NP/posix.a $NP/core.a $NP/sys.a $NP/lib.a -Wl,--no-as-needed -ldl -lpthread #-pthread
 

