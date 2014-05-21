/*
 * Copyright (C) 2014 University of Applied Sceiences Hamburg
 *
 * This file subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @addtogroup  pnet
 * @{
 */

/**
 * @file    sys/select.h
 * @brief   Header specifying posix select
 * @see     <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_select.h.html">
 *              The Open Group Base Specifications Issue 7, <sys/select.h>
 *          </a>
 *
 * @author  University of Applied Sceiences Hamburg (HAW)
 * @author  TBD
 */

#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H

#include <sys/types.h>
#include <signal.h>
#include <time.h>

#define FD_SETSIZE 128 // todo: find decent size

struct fd_set {

};

struct timeval {
    time_t         tv_sec;      ///< Seconds
    suseconds_t    tv_usec;     ///> Microseconds
};


/* The following shall be declared as functions, defined as macros, or both.
 * If functions are declared, function prototypes shall be provided.
 * If implemented as macros, these may evaluate their arguments more than once,
 * so applications should ensure that the arguments they supply are never
 * expressions with side-effects.
 */
void FD_CLR(int, fd_set *);
int  FD_ISSET(int, fd_set *);
void FD_SET(int, fd_set *);
void FD_ZERO(fd_set *);

/**
 * @brief Synchronous I/O multiplexing a la POSIX.1g
 * @details The pselect() function shall examine the file descriptor sets whose
 *          addresses are passed in the readfds, writefds, and errorfds
 *          parameters to see whether some of their descriptors are ready for
 *          reading, are ready for writing, or have an exceptional condition 
 *          pending, respectively.
 * 
 * @see <a href="http://pubs.opengroup.org/onlinepubs/9699919799/functions/pselect.html">
 *          The Open Group Base Specification Issue 7, accept
 *      </a>
 *
 * @param[] nfds        The nfds argument specifies the range of descriptors
 *                      to be tested. The first nfds descriptors shall be
 *                      checked in each set; that is, the descriptors from
 *                      zero through nfds-1 in the descriptor sets shall be
 *                      examined.
 *
 * @param[] readfds     If the readfds argument is not a null pointer, it points
 *                      to an object of type fd_set that on input specifies the 
 *                      file descriptors to be checked for being ready to read,
 *                      and on output indicates which file descriptors are ready
 *                      to read.
 *
 * @param[] writefds    If the writefds argument is not a null pointer, it
 *                      points to an object of type fd_set that on input 
 *                      specifies the file descriptors to be checked for being 
 *                      ready to write, and on output indicates which file
 *                      descriptors are ready to write.
 *
 * @param[] errorfds    If the errorfds argument is not a null pointer, it
 *                      points to an object of type fd_set that on input
 *                      specifies the file descriptors to be checked for error 
 *                      conditions pending, and on output indicates which file
 *                      descriptors have error conditions pending.
 *
 * @param[] timeout
 *
 * @param[] sigmask
 */
int pselect(int nfds,
            fd_set *restrict readfds,
            fd_set *restrict writefds,
            fd_set *restrict errorfds,
            const struct timespec *restrict timeout,
            const sigset_t *restrict sigmask);

/**
 * @brief Synchronous I/O multiplexing
 * @details The select() function shall be equivalent to the pselect() function,
 *          except as follows: (1) For the select() function, the timeout period
 *          is given in seconds and microseconds in an argument of type struct
 *          timeval, whereas for the pselect() function the timeout period is
 *          given in seconds and nanoseconds in an argument of type struct
 *          timespec. (2) The select() function has no sigmask argument; it 
 *          shall behave as pselect() does when sigmask is a null pointer.
 *          (3) Upon successful completion, the select() function may modify 
 *          the object pointed to by the timeout argument.
 * 
 * @see <a href="http://pubs.opengroup.org/onlinepubs/9699919799/functions/select.html">
 *          The Open Group Base Specification Issue 7, accept
 *      </a>
 *
 * @param[] nfds        The nfds argument specifies the range of descriptors
 *                      to be tested. The first nfds descriptors shall be
 *                      checked in each set; that is, the descriptors from
 *                      zero through nfds-1 in the descriptor sets shall be
 *                      examined.
 *
 * @param[] readfds     If the readfds argument is not a null pointer, it points
 *                      to an object of type fd_set that on input specifies the 
 *                      file descriptors to be checked for being ready to read,
 *                      and on output indicates which file descriptors are ready
 *                      to read.
 *
 * @param[] writefds    If the writefds argument is not a null pointer, it
 *                      points to an object of type fd_set that on input 
 *                      specifies the file descriptors to be checked for being 
 *                      ready to write, and on output indicates which file
 *                      descriptors are ready to write.
 *
 * @param[] errorfds    If the errorfds argument is not a null pointer, it
 *                      points to an object of type fd_set that on input
 *                      specifies the file descriptors to be checked for error 
 *                      conditions pending, and on output indicates which file
 *                      descriptors have error conditions pending.
 *
 * @param[] timeout
 *
 */
int select(int nfds,
           fd_set *restrict readfds,
           fd_set *restrict writefds,
           fd_set *restrict errorfds,
           struct timeval *restrict timeout);

/**
 * @}
 */
#endif /* _SYS_SELECT_H */
