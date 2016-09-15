
#include "variety.h"
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

_WCRTLINK int pthread_once(pthread_once_t *__once_control, void (*__init_routine)(void))
{
    if(pthread_mutex_lock(&__once_control->access) == 0) {
        if(__once_control->executed == 0) {
            __init_routine();
            __once_control->executed = 1;
        }
        pthread_mutex_unlock(&__once_control->access);
    } else
        return( EBUSY );
    
    return( 0 );
}
