
#include "variety.h"
#include <sys/types.h>

_WCRTLINK int pthread_equal( pthread_t __p1, pthread_t __p2 )
{
    if(__p1.id == __p2.id)
        return 1;
        
    return 0;
}
