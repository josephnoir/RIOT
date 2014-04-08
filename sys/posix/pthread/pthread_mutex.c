/**
 * POSIX implementation of threading.
 *
 * Copyright (C) 2013 Freie Universität Berlin
 *
 * This file subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 *
 * @ingroup posix
 * @{
 * @file    pthread.c
 * @brief   Implementation of pthread.
 * @author  Christian Mehlis <mehlis@inf.fu-berlin.de>
 * @author  René Kijewski <kijewski@inf.fu-berlin.de>
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "pthread.h"

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
    (void) mutexattr;

    return (mutex_init(mutex) == 1 ? 0 : -1);
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    return (mutex_trylock(mutex) == 1 ? 0 : -1);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    return (mutex_lock(mutex) == 1 ? 0 : -1);
}

int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abstime)
{
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) mutex;
    (void) abstime;
    return -1; /* currently not supported */
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    mutex_unlock(mutex);
    return 0;
}

int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *prioceiling)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) mutex;
    (void) prioceiling;
    return 0;
}

int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) mutex;
    (void) prioceiling;
    (void) old_ceiling;
    return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    return 0;
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) pshared;
    return 0;
}

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) pshared;
    return 0;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *kind)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) kind;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) kind;
    return 0;
}

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) protocol;
    return 0;
}

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) protocol;
    return 0;
}

int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) prioceiling;
    return 0;
}

int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) prioceiling;
    return 0;
}

int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robustness)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) robustness;
    return 0;
}

int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robustness)
{
    // TODO
    printf("%s:%d  %s", __FILE__, __LINE__,__PRETTY_FUNCTION__);
    (void) attr;
    (void) robustness;
    return 0;
}
