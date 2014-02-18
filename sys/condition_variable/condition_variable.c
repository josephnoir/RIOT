/*
 * Copyright (C) 2014
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     sys
 * @{
 *
 * @file        condition_variable.c
 * @brief       Condition variable implementation
 *
 * @author      
 *
 * @}
 */

 #include "condition_variable.h"

int pthread_cond_init(struct pthread_cond_t cond*, struct condattr_t attr*);
{
	return 1;
}

int pthread_cond_destroy(struct pthread_cond_t cond*);
{
	return 1;
}

int pthread_cond_wait(struct pthread_cond_t cond*, struct mutex_t *mutex);
{
	return 1;
}

int pthread_cond_timed_wait(struct pthread_cond_t cond*, struct mutex_t *mutex, long milisec);
{
	return 1;
}

int pthread_cond_signal(struct pthread_cond_t cond*);
{
	return 1;
}

int pthread_cond_broadcast(struct pthread_cond_t cond*);
{
	return 1;
}
