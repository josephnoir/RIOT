
#define PTHREAD_RWLOCK_INITIALIZER NULL

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));

int pthread_attr_getstack(const pthread_attr_t * attr, void ** stackaddr, size_t * stacksize);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);

void pthread_cleanup_push(void (*routine)(void*), void *arg);
void pthread_cleanup_pop(int execute);


/*
 * #ifdef __USE_UNIX98
 */
int pthread_getconcurrency(void);
int pthread_setconcurrency(int new_level);
int pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id);
