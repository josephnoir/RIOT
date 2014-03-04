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
#include <exception>
#include "pthread.h"


void foo()
{
    printf("I am f00!\n");
}

int main() {

   printf("Hello c++\n");

  //void * ptr1 = (void *) &pthread_create;
  void * ptr2 = (void *) &pthread_join;
//printf("%p\n", ptr2);

    std::thread bar (foo);
std::thread::id x = bar.get_id();
printf("%p\n", &x);

//thread_yield();

try{
   bar.join();
}catch(std::exception& e)
 {
   printf("%s\n", e.what());
  }  

  printf("Done\n");
}

