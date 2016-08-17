
#include "variety.h"
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

_WCRTLINK int pthread_barrier_init( pthread_barrier_t *__barrier,
                                    const pthread_barrierattr_t *__attr, 
                                    unsigned __count )
{

    if(__barrier == NULL)
        return( EINVAL );

    __barrier->access = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if(__barrier->access == NULL)
        return( ENOMEM );
    pthread_mutex_init(__barrier->access, NULL);
    
    __barrier->cond = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    if(__barrier->cond == NULL) {
        free(__barrier->access);
        return( ENOMEM );
    }
    pthread_cond_init(__barrier->cond, NULL);

    __barrier->count = 0;
    __barrier->limit = __count;

    return( 0 );
}

_WCRTLINK int pthread_barrier_destroy(pthread_barrier_t *__barrier)
{
int res;

    pthread_mutex_lock(__barrier->access);
    if(__barrier->count > 0) {
        pthread_mutex_unlock(__barrier->access);
        return( EBUSY );
    }
    
    res = pthread_cond_destroy(__barrier->cond);
    if(res != 0) {
        pthread_mutex_unlock(__barrier->access);
        return( res );
    }
    
    pthread_mutex_unlock(__barrier->access);
    pthread_mutex_destroy(__barrier->access);
    
    return( 0 );
}

_WCRTLINK int pthread_barrier_wait(pthread_barrier_t *__barrier)
{
    pthread_mutex_lock(__barrier->access);
    
    __barrier->count++;
    
    if(__barrier->count >= __barrier->limit) {
        pthread_cond_broadcast(__barrier->cond);
        __barrier->count = 0;
    } else {
        pthread_cond_wait(__barrier->cond, __barrier->access);
    }
    
    pthread_mutex_unlock(__barrier->access);
    
    return( 0 );
}
