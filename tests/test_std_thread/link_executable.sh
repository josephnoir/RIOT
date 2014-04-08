#!/bin/bash

GOBACK=$PWD
cd shared
#rm libstartup.so
#rm libuart0.so

for i in lib*.so ; do libs="$PWD/$i $libs " ; done



#echo $libs
cd $GOBACK
PTH=$PWD/bin/native

# $PTH/pthread.a $PTH/cpu.a $PTH/native_base.a   $PTH/uart0.a $PTH/posix.a $PTH/core.a  $PTH/sys.a $PTH/lib.a $PTH/test_std_thread.a
# $PTH/pthread.a $PTH/cpu.a $PTH/native_base.a   $PTH/uart0.a $PTH/posix.a $PTH/core.a  $PTH/sys.a $PTH/lib.a
#g++ -g -std=c++11 -static-libstdc++ -static-libgcc main.cpp -o wtf.elf -Wl,--whole-archive $PTH/pthread.a -Wl,--no-whole-archive $PTH/cpu.a $PTH/timex.a $PTH/vtimer.a $PTH/native_base.a $PTH/uart0.a $PTH/posix.a $PTH/core.a $PTH/sys.a $PTH/lib.a $PTH/test_std_thread.a  $lib -Wl,--no-as-needed -ldl
#-static-libstdc++
g++ -g -std=c++11 -static-libstdc++ -static-libgcc main.cpp -o wtf.elf -Wl,--whole-archive theone.a -Wl,--no-whole-archive -Wl,--no-as-needed -ldl