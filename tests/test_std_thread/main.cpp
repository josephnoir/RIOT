/*
 * Copyright (C) 2014 
 * Copyright (C) 2014 
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @file
 * @brief       Demonstration of mixed c++ user application with pure c RIOT
 *
 * @author      
 *
 */

#include <cstdio>
#include <thread>
#include <chrono>
#include <typeinfo>
#include <exception>
extern "C" {
    #include "pthread.h"
};


using namespace std;

void foo()
{
    printf("I am f00!\n");
    for(long i = 0; i < 10000000; ++i);
    this_thread::yield(); 
    //thread_yield();
    for(long i = 0; i < 10000000; ++i);
    this_thread::yield();
    //thread_yield();
    for(long i = 0; i < 10000000; ++i);
    printf("f00 done\n");
}

void bar(thread* other)
{
    printf("I am bar!\n");
    try {
        other->join();
    } catch(std::exception& e) {
        printf("(join t1) what: %s, typeid %s\n", e.what(), typeid(e).name());
    }
    printf("bar done\n");
}

int main() {

    printf("Hello c++\n");

    thread t1 (foo);
    thread t2 (bar, &t1);
    
    try {
        t2.join();
    } catch(std::exception& e) {
        printf("(join t2) what: %s, typeid %s\n", e.what(), typeid(e).name());
    }
    printf("after join t2\n");
    
    printf("Done\n");
    return 0;
}

