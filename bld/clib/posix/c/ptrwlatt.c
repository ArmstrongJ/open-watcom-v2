
#include "variety.h"
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

_WCRTLINK int pthread_rwlockattr_init(pthread_rwlockattr_t *__attr)
{
    return( 0 );
}

_WCRTLINK int pthread_rwlockattr_destroy(pthread_rwlockattr_t *__attr)
{
    return( 0 );
}

_WCRTLINK int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__attr, int *__pshared)
{
    if(__attr == NULL || __pshared == NULL)
        return( EINVAL );
        
    *__pshared = PTHREAD_PROCESS_PRIVATE;
    
    return( 0 );
}

_WCRTLINK int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *__attr, int __pshared)
{
    if(__attr == NULL)
        return( EINVAL );
    
    if(__pshared != PTHREAD_PROCESS_PRIVATE)
        return( ENOSYS );
        
    return( 0 );
}
