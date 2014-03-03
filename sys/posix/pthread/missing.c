
#include "pthread.h"


int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    (void) prepare;
    (void) parent;
    (void) child;
	return 0;
}

int pthread_attr_getstack(const pthread_attr_t * attr, void ** stackaddr, size_t * stacksize)
{
    (void) attr;
    (void) stackaddr;
    (void) stacksize;
    return 0;
}


int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
    (void) attr;
    (void) stackaddr;
    (void) stacksize;
    return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier);
{
    (void) barrier;
    return 0;
}

int pthread_barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned count)
{
    (void) barrier;
    (void) attr;
    (void) count;
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
    (void) barrier;
    return 0;    
}

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr)
{
    (void) attr;
    return 0;
}

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *  attr, int * pshared)
{
    (void) attr;
    (void) pshared;
    return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared)
{
    (void) attr;
    (void) pshared;
    return 0;
}


int pthread_barrierattr_init(pthread_barrierattr_t *attr)
{
    (void) attr;
    return 0;
}

void pthread_cleanup_push(void (*routine)(void*), void *arg)
{
    (void) routine;
    (void) arg;    
}

void pthread_cleanup_pop(int execute)
{
    (void) execute;
}

int pthread_getconcurrency(void)
{
    return 0;
}

int pthread_setconcurrency(int new_level)
{
    (void) new_level;
    return 0;
}

int pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id)
{
    (void) thread_id;
    (void) clock_id;
    return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
    (void) rwlock;
    (void) attr;
    return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock, const struct timespec * abs_timeout)
{
    (void) rwlock;
    (void) abs_timeout;
    return 0;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock, const struct timespec * abs_timeout)
{
    (void) rwlock;
    (void) abs_timeout;
    return 0;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    (void) rwlock;
    return 0;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    (void) attr;
    return 0;
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *  attr, int * pshared)
{
    (void) attr;
    (void) pshared;
    return 0;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
{
    (void) attr;
    (void) pshared;
    return 0;
}

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    (void) attr;
    return 0;
}


