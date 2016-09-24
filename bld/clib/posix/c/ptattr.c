
#include "variety.h"
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>


_WCRTLINK int pthread_attr_init(pthread_attr_t *__attr)
{
    if( __attr == NULL )
        return( EINVAL );
    
    __attr->stack_size = 0;
    __attr->stack_addr = NULL;
    __attr->detached = PTHREAD_CREATE_JOINABLE;
    __attr->sched_inherit = 1;
    __attr->sched_policy = SCHED_NORMAL;
    __attr->sched_params = NULL;
    
    return( 0 );
    
}

_WCRTLINK int pthread_attr_destroy(pthread_attr_t *__attr)
{
    if( __attr == NULL )
        return( EINVAL );
    
    if( __attr->sched_params != NULL )
        free(__attr->sched_params);
        
    __attr->sched_params = NULL;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_setdetachstate(pthread_attr_t *__attr, int detachstate)
{
    /* Detaching really isn't necessary in Open Watcom's
     * pthread implementation, but you can set this...
     * 
     * See ptdetach.c for more info
     */
     
    if( __attr == NULL || (detachstate != PTHREAD_CREATE_JOINABLE && detachstate != PTHREAD_CREATE_DETACHED))
        return( EINVAL );
        
    __attr->detached = detachstate;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getdetachstate(const pthread_attr_t *__attr, int *detachstate)
{
    if( __attr == NULL || detachstate == NULL )
        return( EINVAL );
    
    *detachstate = __attr->detached;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getguardsize(const pthread_attr_t *__attr, size_t *guardsize)
{
    if( __attr == NULL || guardsize == NULL)
        return( EINVAL );
    
    *guardsize = 0;

    return( 0 );
}

_WCRTLINK int pthread_attr_setguardsize(pthread_attr_t *__attr, size_t guardsize)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_attr_setinheritsched(pthread_attr_t *__attr, int inheritsched)
{
    if( __attr == NULL || (inheritsched != PTHREAD_EXPLICIT_SCHED && inheritsched != PTHREAD_INHERIT_SCHED))
        return( EINVAL );

    __attr->sched_inherit = inheritsched;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getinheritsched(const pthread_attr_t *__attr, int *inheritsched)
{
    if( __attr == NULL || inheritsched == NULL )
        return( EINVAL );

    *inheritsched = __attr->sched_inherit;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_setschedpolicy(pthread_attr_t *__attr, int policy)
{
    if( __attr == NULL )
        return( EINVAL );
        
    if( policy != SCHED_FIFO ||
        policy != SCHED_RR ||
        policy != SCHED_BATCH ||
        policy != SCHED_IDLE ||
        policy != SCHED_DEADLINE) 
    {
        return( EINVAL );
    }
    
    __attr->sched_policy = policy;

    return( 0 );
}

_WCRTLINK int pthread_attr_getschedpolicy(const pthread_attr_t *__attr, int *policy)
{
    if( __attr == NULL || policy == NULL )
        return( EINVAL );

    *policy = __attr->sched_policy;
    
    return( 0 );
}


_WCRTLINK int pthread_attr_setschedparam(pthread_attr_t *__attr, const struct sched_param *__params)
{
    if( __attr == NULL )
        return( EINVAL );

    if(__params == NULL && __attr->sched_params != NULL ) {
        free(__attr->sched_params);
        __attr->sched_params = NULL;
    } else if(__params != NULL) {
        if( __attr->sched_params == NULL )
            __attr->sched_params = (struct sched_param *)malloc(sizeof(struct sched_param));
        if( __attr->sched_params == NULL )
            return( ENOMEM );
            
        memcpy(__attr->sched_params, __params, sizeof(struct sched_param));
    }
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getschedparam(const pthread_attr_t *__attr, struct sched_param *__params)
{
    if( __attr == NULL || __params == NULL )
        return( EINVAL );

    memcpy(__params, __attr->sched_params, sizeof(struct sched_param));
    
    return( 0 );
}

_WCRTLINK int pthread_attr_setscope(pthread_attr_t *__attr, int contentionscope)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_attr_getscope(const pthread_attr_t *__attr, int *contentionscope)
{
    if( __attr == NULL || contentionscope == NULL )
        return( EINVAL );

    *contentionscope = PTHREAD_SCOPE_PROCESS;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_setstackaddr(pthread_attr_t *__attr, void *__stackaddr)
{
    if( __attr == NULL )
        return( EINVAL );
    
    __attr->stack_addr = __stackaddr;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getstackaddr(const pthread_attr_t *__attr, void **__stackaddr)
{
    if( __attr == NULL || __stackaddr == NULL )
        return( EINVAL );
    
    *__stackaddr = __attr->stack_addr;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_setstacksize(pthread_attr_t *__attr, size_t __stacksize)
{
    if( __attr == NULL )
        return( EINVAL );
        
    __attr->stack_size = __stacksize;
    
    return( 0 );
}

_WCRTLINK int pthread_attr_getstacksize(const pthread_attr_t *__attr, size_t *__stacksize)
{
    if( __attr == NULL || __stacksize == NULL )
        return( EINVAL );
    
    *__stacksize = __attr->stack_size;
    
    return( 0 );
}
