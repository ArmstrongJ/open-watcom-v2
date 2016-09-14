
#include "variety.h"
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

_WCRTLINK int pthread_mutexattr_init(pthread_mutexattr_t *__attr)
{
    if(__attr == NULL)
        return( EINVAL );
        
    __attr->type = PTHREAD_MUTEX_DEFAULT;
    
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_destroy(pthread_mutexattr_t *__attr)
{
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *__attr, int __prioceiling)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *__attr, int *__prioceiling)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_mutexattr_setprotocol(pthread_mutexattr_t *__attr, int __protocol)
{
    return( ENOSYS );
}

_WCRTLINK int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__attr, int *__protocol)
{
    if(__protocol == NULL)
        return( EINVAL );
        
    __protocol = PTHREAD_PRIO_NONE;
    
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__attr, int *__pshared)
{
    if(__attr == NULL || __pshared == NULL)
        return( EINVAL );
        
    *__pshared = PTHREAD_PROCESS_PRIVATE;
    
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_setpshared(pthread_mutexattr_t *__attr, int __pshared)
{
    if(__attr == NULL)
        return( EINVAL );
    
    if(__pshared != PTHREAD_PROCESS_PRIVATE)
        return( ENOSYS );
        
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_gettype(const pthread_mutexattr_t *__attr, int *__type)
{
    if(__attr == NULL || __type == NULL)
        return( EINVAL );
        
    *__type = __attr->type;
    return( 0 );
}

_WCRTLINK int pthread_mutexattr_settype(pthread_mutexattr_t *__attr, int __type)
{
    if(__attr == NULL)
        return( EINVAL );
        
    if(__type != PTHREAD_MUTEX_DEFAULT &&
       __type != PTHREAD_MUTEX_RECURSIVE &&
       __type != PTHREAD_MUTEX_ERRORCHECK &&
       __type != PTHREAD_MUTEX_NORMAL)
    {
        return( EINVAL );
    }
    
    /* This implementation can't meet the requirements of
     * this type (same # of locks and unlocks...)
     */
    if(__type == PTHREAD_MUTEX_RECURSIVE)
        return( ENOSYS );
        
    __attr->type = __type;
    
    return( 0 );
}   
