/*
 * Copyright (C) 2014 
 *
 * This file subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    
 * @brief       
 * @ingroup     sys
 * @{
 *
 * @file        condition_variable.h
 * @brief       RIOT POSIX condition variable API
 *
 * @author      
 */

#ifndef _CONDITION_VARIABLE_H
#define _CONDITION_VARIABLE_H

#include "mutex.h"

typedef struct condattr_t {
	// no attributes by now
}condattr_t;


typedef struct pthread_cond_t {
    /* fields are managed by cv functions, don't touch */
    unsigned int val;       // @internal
    queue_node_t queue;     // @internal
}pthread_cond_t;

/**
 * @brief Initializes a condition attribute variable object using default values
 * @param attr pre-allocated condition attribute variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_condattr_destroy(condattr_t *attr);

/**
 * @brief Uninitializes a condition attribute variable object
 * @param attr pre-allocated condition attribute variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_condattr_init(condattr_t *attr);

/**
 * @brief Initializes a condition variable object
 * @param cond pre-allocated condition variable structure.
 * @param attr pre-allocated condition attribute variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_init(struct pthread_cond_t *cond, struct condattr_t *attr);

/**
 * @brief Destroy the condition variable cond
 * @param cond pre-allocated condition variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_destroy(struct pthread_cond_t *cond);

/**
 * @brief blocks the calling thread until the specified condition cond is signalled
 * @param cond pre-allocated condition variable structure.
 * @param mutex pre-allocated mutex variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_wait(struct pthread_cond_t *cond, struct mutex_t *mutex);

/**
 * @brief blocks the calling thread until the specified condition cond is signalled
 * @param cond pre-allocated condition variable structure.
 * @param mutex pre-allocated mutex variable structure.
 * @param abstime pre-allocated timeout.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_timed_wait(struct pthread_cond_t *cond, struct mutex_t *mutex, const struct timespec *abstime);

/**
 * @brief unblock at least one of the threads that are blocked on the specified condition variable cond
 * @param cond pre-allocated condition variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_signal(struct pthread_cond_t *cond);

/**
 * @brief unblock all threads that are currently blocked on the specified condition variable cond
 * @param cond pre-allocated condition variable structure.
 * @return Always returns 0, always succeeds.
 */
int pthread_cond_broadcast(struct pthread_cond_t *cond);

/** @} */
#endif /* _CONDITION_VARIABLE_H */
