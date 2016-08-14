
#include "variety.h"
#include <sys/types.h>
#include "_ptint.h"

#ifdef __UNIX__
#include <signal.h>
#include <errno.h>
#endif

_WCRTLINK int pthread_cancel( pthread_t thread )
{
#ifdef __UNIX__
int ret;
    ret = kill(thread.id, SIGCANCEL);
    if(ret != 0)
        ret = errno;

    return( ret );
#else
    return( ENOSYS );
#endif
}
