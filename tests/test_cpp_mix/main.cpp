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
#include "pthread.h"
#include "cppMix.hpp"

int main() {
    printf("Hello c++\n");
printf("%p\n", (void *)&pthread_create);
    cppMix oMix;
    oMix.sayHello();
        
}
