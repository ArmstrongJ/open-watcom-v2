#include "variety.h"
#include <pthread.h>
#include <sys/types.h>
#include <sched.h>
#include "_ptint.h"

_WCRTLINK int pthread_getschedparam(pthread_t __thr, int *__policy, struct sched_param *__param)
{
int ret;
pid_t tid;

    ret = 0;

    tid = __get_thread_id(__thr);

    if(__policy != NULL)
        *__policy = sched_getscheduler(tid);
    
    if(__param != NULL)
        ret = sched_getparam(tid, __param);

    return( ret );
}


_WCRTLINK int pthread_setschedparam(pthread_t __thr, int __policy, const struct sched_param *__param)
{
pid_t tid;

    tid = __get_thread_id(__thr);

    return( sched_setscheduler(tid, __policy, __param) );
}
