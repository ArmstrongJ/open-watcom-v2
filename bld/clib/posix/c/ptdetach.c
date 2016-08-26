
#include "variety.h"
#include <sys/types.h>

_WCRTLINK int pthread_detach(pthread_t __thr)
{
    /* This routine is unnecessary with Open Watcom's
     * thread implementation, but we'll implement a
     * dummy function for completeness
     */
    
    return( 0 );
}
