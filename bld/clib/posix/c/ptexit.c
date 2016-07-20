
#include "variety.h"
#include <pthread.h>
#include <process.h>

#include "_ptint.h"

_WCRTLINK void pthread_exit(void *value_ptr)
{
pthread_t *myself;

    myself = __get_current_thread( );
    __unregister_thread(myself);
    
    /* This routine also needs to notify waiting threads */
    _endthread();
}    
