
#include "variety.h"
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "_ptint.h"

#ifdef __UNIX__
#include <signal.h>
#include <errno.h>
#endif

/* Internal flag for indicating defered cancellation */
#define PTHREAD_CANCEL_SET  128

/* Cancellation status */
#define CANCEL_ENABLED(t)   ((t->cancel_status & PTHREAD_CANCEL_ENABLE) == PTHREAD_CANCEL_ENABLE)
#define CANCEL_DEFERED(t)   ((t->cancel_status & PTHREAD_CANCEL_DEFERRED) == PTHREAD_CANCEL_DEFERRED)
#define CANCEL_REQUESTED(t) ((t->cancel_status & PTHREAD_CANCEL_SET) == PTHREAD_CANCEL_SET)

typedef void (*sighandler_t)(int);

static void __thread_handle_cancellation(int signal)
{
    pthread_exit(NULL);
}

int __thread_enable_cancellation(int enable)
{
sighandler_t res;

    if(enable == PTHREAD_CANCEL_ENABLE)
        res = signal(SIGCANCEL, __thread_handle_cancellation);
    else
        res = signal(SIGCANCEL, SIG_IGN);
        
    return( res == __thread_handle_cancellation ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE );
}

_WCRTLINK int pthread_cancel( pthread_t __thread )
{
pthread_t *internal;
int ret;

    /* Return the internal representation of the thread
     * to ensure accurate status flags
     */
    internal = __get_thread(__thread.id);
    
    if(!CANCEL_ENABLED(internal))
        return( EPERM );
    
    if(CANCEL_DEFERED(internal))
        internal->cancel_status |= PTHREAD_CANCEL_SET;
    else {
#ifdef __UNIX__
        ret = kill(__thread.id, SIGCANCEL);
        if(ret != 0)
            ret = errno;
    
#else
        ret = ENOSYS;
#endif
    }
    
    return( ret );
}

_WCRTLINK int pthread_setcancelstate(int __state, int *__oldstate)
{
pthread_t *internal;
int res;

    internal = __get_current_thread();

    if(__state != PTHREAD_CANCEL_ENABLE ||
       __state != PTHREAD_CANCEL_DISABLE)
    {
        return( EINVAL );
    }

    res = internal->cancel_status & PTHREAD_CANCEL_ENABLE;
    if(res != __state) {
        if(res == PTHREAD_CANCEL_ENABLE)
            internal->cancel_status -= PTHREAD_CANCEL_ENABLE;
        else
            internal->cancel_status += PTHREAD_CANCEL_ENABLE;
    }
    
    if(__oldstate != NULL)
        *__oldstate = res;
        
    return( 0 );
}

_WCRTLINK int pthread_setcanceltype(int __type, int *__oldtype)
{
pthread_t *internal;
int res;

    internal = __get_current_thread();

    if(__type != PTHREAD_CANCEL_DEFERRED ||
       __type != PTHREAD_CANCEL_ASYNCHRONOUS)
    {
        return( EINVAL );
    }

    res = CANCEL_DEFERED(internal) ? PTHREAD_CANCEL_DEFERRED : PTHREAD_CANCEL_ASYNCHRONOUS;

    if(res != __type) {
        if(__type == PTHREAD_CANCEL_DEFERRED) {
            internal->cancel_status += PTHREAD_CANCEL_DEFERRED;
            internal->cancel_status -= PTHREAD_CANCEL_ASYNCHRONOUS;
            signal(SIGCANCEL, SIG_IGN);
        } else {
            internal->cancel_status -= PTHREAD_CANCEL_DEFERRED;
            internal->cancel_status += PTHREAD_CANCEL_ASYNCHRONOUS;
            signal(SIGCANCEL, __thread_handle_cancellation);
        }
    }

    if(__oldtype != NULL)
        *__oldtype = res;

    return( 0 );
}

_WCRTLINK void pthread_testcancel(void)
{
pthread_t *internal;

    internal = __get_current_thread();    
    if(CANCEL_ENABLED(internal) && CANCEL_REQUESTED(internal))
        __thread_handle_cancellation(0);
}
