/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 2016 Open Watcom Contributors.
*    All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  POSIX thread mutual exclusion handling
*
* Author: J. Armstrong
*
****************************************************************************/

#define MUTEX_STATUS_READY      100
#define MUTEX_STATUS_LOCKED     101
#define MUTEX_STATUS_DESTROYED  200

#include "variety.h"
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <process.h>
#include "rterrno.h"
#include "thread.h"


#include "_ptint.h"

extern int __atomic_compare_and_swap( volatile int *__dest, int __expected, int __source );

_WCRTLINK int pthread_mutex_init(pthread_mutex_t *__mutex, const pthread_mutexattr_t *__attr)
{
    if(__mutex == NULL)
        return( EINVAL );

    __mutex->access = (sem_t *)malloc(sizeof(sem_t));
    __mutex->mutex = (sem_t *)malloc(sizeof(sem_t));
    if(__mutex->access == NULL || __mutex->mutex == NULL)
        return( _RWD_errno );

    if(sem_init(__mutex->access, 0, 1) != 0) {
        return( _RWD_errno );
    }
    
    if(sem_init(__mutex->mutex, 0, 1) != 0) {
        return( _RWD_errno );
    }
    
    __mutex->status = (int *)malloc(sizeof(int));
    *__mutex->status = MUTEX_STATUS_READY;
    __mutex->owner = (pid_t *)malloc(sizeof(pid_t));
    *__mutex->owner = (pid_t)-1;
    __mutex->type = (int *)malloc(sizeof(int));
    *__mutex->type = PTHREAD_MUTEX_DEFAULT;
    
    if(__attr != NULL)
        *__mutex->type = __attr->type;
    
    return( 0 );   
}

_WCRTLINK int pthread_mutex_destroy(pthread_mutex_t *__mutex)
{
int res;

    if(__mutex->status != MUTEX_STATUS_READY)
        return( EINVAL );
    
    /* Need to ensure the mutex isn't currently locked */
    res = pthread_mutex_trylock(__mutex);
    if(res != 0)
        return( res );

    if(sem_destroy(__mutex->access) != 0)
        return( _RWD_errno );
    
    /* Need to release the mutex semaphore now */
    sem_post(__mutex->mutex);
    if(sem_destroy(__mutex->mutex) != 0)
        return( _RWD_errno );
        
    free(__mutex->access);
    free(__mutex->mutex);
        
    *__mutex->status = MUTEX_STATUS_DESTROYED;
    return( 0 );
}

#define LOCK_PROCEED        0
#define LOCK_ERROR_OWNED    1

static int __pthread_check_lock_type(pthread_mutex_t *__mutex)
{
int ret;

    ret = LOCK_PROCEED;

    sem_wait(__mutex->access);
    if(*__mutex->status == MUTEX_STATUS_READY)
        ret = LOCK_PROCEED;
    else if(*__mutex->status == MUTEX_STATUS_LOCKED && __mutex->owner == gettid()) {
    
        /* For a "normal" mutex, proceed with deadlock... */
        if(*__mutex->type == PTHREAD_MUTEX_NORMAL)
            ret = LOCK_PROCEED;
        
        else if(*__mutex->type == PTHREAD_MUTEX_ERRORCHECK)
            ret = LOCK_ERROR_OWNED;
    }    
    sem_post(__mutex->access);
    
    return( ret );
}

_WCRTLINK int pthread_mutex_trylock(pthread_mutex_t *__mutex)
{
int ret;

    if(__mutex == NULL)
        return( EINVAL );

    sem_wait(__mutex->access);
    if(__mutex->status == MUTEX_STATUS_READY) {
        sem_wait(__mutex->mutex);
        *__mutex->status = MUTEX_STATUS_LOCKED;
        *__mutex->owner = gettid();
        ret = 0;
    } else 
        ret = EBUSY;
    
    sem_post(__mutex->access);
    return( ret );
}

_WCRTLINK int pthread_mutex_lock(pthread_mutex_t *__mutex)
{
int ret;
int res;
    
    if(__mutex == NULL)
        return( EINVAL );
    
    ret = -1;
    
    /* res = __pthread_check_lock_type(__mutex);
     * if(res == LOCK_ERROR_OWNED)
     *     return( EDEADLK );
     */
    
    if(sem_wait(__mutex->mutex) == 0) {
        //sem_wait(__mutex->access);
        
        res = __atomic_compare_and_swap( __mutex->status, MUTEX_STATUS_READY, MUTEX_STATUS_LOCKED);
        if(res == 1) {
            res = __atomic_compare_and_swap( __mutex->owner, -1, gettid());
            if(res == 0) {
                __atomic_compare_and_swap( __mutex->status, MUTEX_STATUS_LOCKED, MUTEX_STATUS_READY);
                ret = EPERM;
                printf("Lock failed\n");
                
            } else
                ret = 0;
        } else
            ret = EPERM;
        
        //sem_post(__mutex->access);
    } 
    
    if(ret != 0) {
        sem_getvalue(__mutex->mutex, &res);
        printf("Fail: %d - tid=%d, status=%d, owner=%d, mutex sem=%d\n", ret, gettid(), *__mutex->status, *__mutex->owner, res);
    }
    sched_yield();
    return( ret );
}

_WCRTLINK int pthread_mutex_unlock(pthread_mutex_t *__mutex)
{
int ret;
int res;

    if(__mutex == NULL)
        return( EINVAL );
    
    ret = -1;
    //while(sem_wait(__mutex->access) != 0);
        
    if(*__mutex->owner == -1) {
        printf("%d -> %d\n", *__mutex->owner, gettid());
        //exit(66);
        __atomic_compare_and_swap( __mutex->status, MUTEX_STATUS_LOCKED, MUTEX_STATUS_READY);
        return EPERM;
    }
    
    if(*__mutex->status == MUTEX_STATUS_READY)
        ret = 0;
    else if(*__mutex->owner == gettid()) {
        //printf("match unlock\n");
        if(!__atomic_compare_and_swap( __mutex->owner, gettid(), -1))
            printf("unlock failure\n");
        if(!__atomic_compare_and_swap( __mutex->status, MUTEX_STATUS_LOCKED, MUTEX_STATUS_READY))
            printf("unlock failure part 2\n");
        //__mutex->status = MUTEX_STATUS_READY;
        //__mutex->owner = (pid_t)-1;
        ret = 0;
        
        sem_post(__mutex->mutex);
        sched_yield();

    } else if(*__mutex->status == MUTEX_STATUS_LOCKED) {
        ret = EPERM;
        printf("EPERM\n");
    } else if(*__mutex->status == MUTEX_STATUS_DESTROYED) {
        ret = EINVAL;
        printf("EINVAL\n");
    } else {
        ret = EBUSY;
        printf("EBUSY\n");
    }

    if(ret != 0) {
        sem_getvalue(__mutex->mutex, &res);
        printf("Unlock Fail: %d - tid=%d, status=%d, owner=%d, mutex sem=%d\n", ret, gettid(), *__mutex->status, *__mutex->owner, res);
    }

    //sem_post(__mutex->access);
    
    return( ret );
}

int __pthread_mutex_mylock(pthread_mutex_t *__mutex)
{
    int ret;

    if(__mutex == NULL)
        return( EINVAL );

    ret = -1;
    if(sem_wait(__mutex->access) == 0) {
        if(*__mutex->status == MUTEX_STATUS_READY)
            ret = -1;
        else if(*__mutex->owner == gettid()) {
            ret = 0;
        } else if(*__mutex->status == MUTEX_STATUS_DESTROYED)
            ret = EINVAL;
        else
            ret = EBUSY;
        
        sem_post(__mutex->access);
    }
    
    return( ret );
}

_WCRTLINK int pthread_mutex_setprioceiling(pthread_mutex_t *__mutex, int __prioceiling, int *__old_ceiling)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_mutex_getprioceiling(const pthread_mutex_t *__mutex, int *__prioceiling)
{
    return( ENOSYS );
}
