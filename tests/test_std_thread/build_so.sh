#!/bin/bash

mkdir -p shared
cd bin/native

for i in *.o
do
    if [[ ! "$i" =~ "pthread" ]]
    then
        gcc $i -shared -o ../../shared/lib${i%.o}.so
    fi
done

for i in *pthread*.o
do
    plibs="$plibs $i"
done

gcc -shared -o ../../shared/libpthread.so $plibs


