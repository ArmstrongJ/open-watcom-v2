
#include "variety.h"
#include <pthread.h>
#include <errno.h>
#include "_ptint.h"

_WCRTLINK int pthread_getconcurrency(void)
{
    return( 0 );
}

_WCRTLINK int pthread_setconcurrency(int new_level)
{
    return( ENOSYS );
}
