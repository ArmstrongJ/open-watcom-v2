
#include "variety.h"
#include <pthread.h>
#include <process.h>
#include <stdio.h>

#include "_ptint.h"

_WCRTLINK void pthread_exit(void *value_ptr)
{
int waiters_local;
pthread_t *myself;

    myself = __get_current_thread( );
    if(myself == NULL) {
        fprintf(stderr, "ERROR: thread was null during de-register\n");
        _endthread();
    }
    __unregister_thread(myself);
    
    /* Unlock to release any joins */
    pthread_mutex_unlock(myself->running_mutex);
    
    /* Wait until all "join" threads have copied our pointer */
    waiters_local = 128;
    while(waiters_local > 0) {
        pthread_mutex_lock(myself->waiting_mutex);
        waiters_local = myself->waiters;
        pthread_mutex_unlock(myself->waiting_mutex);
    }
    
    /* This routine also needs to notify waiting threads */
    _endthread();
}    

