/*
 * Copyright (C) 2014 Hamburg University of Applied Siences (HAW)
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     sys
 * @{
 *
 * @file        pthread_cond.c
 * @brief       Condition variable implementation
 *
 * @author      Martin Landsmann <martin.landsmann@haw-hamburg.de>
 *
 * @}
 */

#include "pthread_cond.h"
#include "thread.h"
#include "vtimer.h"
#include "debug.h"

struct vtimer_t timer;

int pthread_cond_condattr_destroy(struct pthread_condattr_t *attr)
{
    if (attr != NULL) {
        DEBUG("pthread_cond_condattr_destroy: currently attributes are not supported.\n");
    }

    return 0;
}

int pthread_cond_condattr_init(struct pthread_condattr_t *attr)
{
    if (attr != NULL) {
        DEBUG("pthread_cond_condattr_init: currently attributes are not supported.\n");
    }

    return 0;
}

int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
{
    (void)attr;
    (void)pshared;

    return 0;
}

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
    (void)attr;
    (void)pshared;

    return 0;
}

int pthread_condattr_getclock(const pthread_condattr_t *attr, clockid_t *clock_id)
{
    (void)attr;
    (void)clock_id;

    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id)
{
    (void)attr;
    (void)clock_id;

    return 0;
}

int pthread_cond_init(struct pthread_cond_t *cond, struct pthread_condattr_t *attr)
{
    cond->val = 0;

    if (attr != NULL) {
        DEBUG("pthread_cond_init: currently attributes are not supported.\n");
    }

    cond->queue.priority = 0;
    cond->queue.data = 0;
    cond->queue.next = NULL;
    return 0;
}

int pthread_cond_destroy(struct pthread_cond_t *cond)
{
    pthread_cond_init(cond, NULL);
    return 0;
}

int pthread_cond_wait(struct pthread_cond_t *cond, struct mutex_t *mutex)
{
    queue_node_t n;
    n.priority = 0;
    n.data = 0;
    n.next = NULL;

    if (cond->val != 0) {
        cond->val = 0;
        if (n.priority != 0 && n.data != 0) {
            queue_remove(&(cond->queue), &n);
        }
        
        mutex_lock(mutex);
        return 0;
    }
    else {
        if (n.priority == 0 && n.data == 0) {
            n.priority = (unsigned int) active_thread->priority;
            n.data = (unsigned int) active_thread->pid;
            n.next = NULL;
            // potential starving
            queue_priority_add(&(cond->queue), &n);
        }
        else if (n.priority != (unsigned int) active_thread->priority) {
            queue_remove(&(cond->queue), &n);
            n.priority = (unsigned int) active_thread->priority;
            queue_priority_add(&(cond->queue), &n);
        }

        mutex_unlock_and_sleep(mutex);
    }

    // no way to arrive here
    return -1;
}

int pthread_cond_timedwait(struct pthread_cond_t *cond, struct mutex_t *mutex, const struct timespec *abstime)
{
    queue_node_t n;
    n.priority = 0;
    n.data = 0;
    n.next = NULL;

    unsigned char is_sleeping = 0;

    if (cond->val != 0) {
        cond->val = 0;
        if (n.priority == 0 && n.data == 0) {
            queue_remove(&(cond->queue), &n);
        }

        mutex_lock(mutex);
        return 0;
    }
    else {
        if (is_sleeping != 0) {
            //return ETIMEDOUT;
            if (n.priority != 0 && n.data != 0) {
                queue_remove(&(cond->queue), &n);
            }

            mutex_lock(mutex);
            return -2;
        }

        if (n.priority == 0 && n.data == 0) {
            n.priority = (unsigned int) active_thread->priority;
            n.data = (unsigned int) active_thread->pid;
            n.next = NULL;
            // potential starving
            queue_priority_add(&(cond->queue), &n);
        }
        else if (n.priority != (unsigned int) active_thread->priority) {
            queue_remove(&(cond->queue), &n);
            n.priority = (unsigned int) active_thread->priority;
            queue_priority_add(&(cond->queue), &n);
        }

        is_sleeping = 1;
        vtimer_set_wakeup(&timer, (*(timex_t *)(abstime)), active_thread->pid);
        mutex_unlock_and_sleep(mutex);
    }

    // no way to arrive here
    return -1;
}

int pthread_cond_signal(struct pthread_cond_t *cond)
{
    queue_node_t *root = &(cond->queue);
    cond->val = 1;

    if (root->next != NULL) {
        root = root->next;
        thread_wakeup((int)root->data);
    }
    return 0;
}

int pthread_cond_broadcast(struct pthread_cond_t *cond)
{
    queue_node_t *root = &(cond->queue);
    cond->val = 1;

    while (root->next != NULL) {
        root = root->next;
        thread_wakeup((int)root->data);
    }

    return 0;
}
