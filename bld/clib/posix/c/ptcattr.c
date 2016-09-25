
#include "variety.h"
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

_WCRTLINK int pthread_condattr_init(pthread_condattr_t *__attr)
{
    return( 0 );
}

_WCRTLINK int pthread_condattr_destroy(pthread_condattr_t *__attr)
{
    return( 0 );
}

_WCRTLINK int pthread_condattr_getpshared(const pthread_condattr_t *__attr, int *__pshared)
{
    if(__attr == NULL || __pshared == NULL)
        return( EINVAL );
        
    *__pshared = PTHREAD_PROCESS_PRIVATE;
    
    return( 0 );
}

_WCRTLINK int pthread_condattr_setpshared(pthread_condattr_t *__attr, int __pshared)
{
    if(__attr == NULL)
        return( EINVAL );
    
    if(__pshared != PTHREAD_PROCESS_PRIVATE)
        return( ENOSYS );
        
    return( 0 );
}

_WCRTLINK extern int pthread_condattr_getclock( pthread_condattr_t *__attr, clockid_t *clk )
{
    if(__attr == NULL || clk == NULL)
        return( EINVAL );
        
    /* This function always returns CLOCK_MONOTONIC because conditions
     * rely on sem_timedwait, which itself uses CLOCK_MONOTONIC and
     * is not changeable.
     */
    *clk = CLOCK_MONOTONIC;
        
    return( 0 );
}

_WCRTLINK extern int pthread_condattr_setclock( pthread_condattr_t *__attr, clockid_t clk )
{
    if(__attr == NULL)
        return( EINVAL );
    
    if(clk != CLOCK_MONOTONIC)
        return( ENOSYS );
        
    return( 0 );
}
