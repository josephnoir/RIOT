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

#include "pthread.h"

#include <cstdio>
#include <thread>

//#include "pthread.h"


void foo()
{
    printf("I am f00!");
}

int main() {
    printf("Hello c++\n");
    std::thread bar (foo);
    bar.join();
    printf("Done\n");
}

