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

#include "pthread.h"

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

    void * ptr1 = (void *) &pthread_create;
    void * ptr2 = (void *) &pthread_join;
    void * ptr3 = (void *) &pthread_equal;
    printf("ptr1: %p\nptr2: %p\nptr3: %p\n", ptr1, ptr2, ptr3);


    thread t1 (foo);
    thread t2 (bar, &t1);
    
    //std::thread::id x = bar.get_id();
    //printf("bar id pointer: %p\n", &x);


    try {
        t2.join();
    } catch(std::exception& e) {
        printf("(join t2) what: %s, typeid %s\n", e.what(), typeid(e).name());
    }
    printf("after join t2\n");
/*
    try {
        t1.join();
    } catch(std::exception& e) {
        printf("(join t1) what: %s, typeid %s\n", e.what(), typeid(e).name());
    }
    printf("after join t1\n");
*/
    printf("Done\n");
    return 0;
}

