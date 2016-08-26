#include "variety.h"
#include <pthread.h>
#include <sys/types.h>
#include <sched.h>

_WCRTLINK int pthread_getschedparam(pthread_t __thr, int *__policy, struct sched_param *__param)
{
int ret;

    ret = 0;

    if(__policy != NULL)
        *__policy = sched_getscheduler(__thr.id);
    
    if(__param != NULL)
        ret = sched_getparam(__thr.id, __param);

    return( ret );
}


_WCRTLINK int pthread_setschedparam(pthread_t __thr, int __policy, const struct sched_param *__param)
{
    return( sched_setscheduler(__thr.id, __policy, __param) );
}
