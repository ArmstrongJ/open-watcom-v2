#include "variety.h"
#include <pthread.h>
#include "_ptint.h"

_WCRTLINK pthread_t pthread_self( )
{
pthread_t ret;

    ret = __get_current_thread( );
    if(ret == (pthread_t)-1)
        ret = (pthread_t)gettid();

    return ret;
}
