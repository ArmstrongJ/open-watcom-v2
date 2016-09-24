
#include "variety.h"
#include <pthread.h>
#include <process.h>
#include <stdio.h>

#include "_ptint.h"

_WCRTLINK void pthread_exit(void *value_ptr)
{
int waiters_local;
pthread_t myself;

    /* Call the thread cleanup routines */
    __call_all_pthread_cleaners( );

    myself = __get_current_thread( );
    if(myself == NULL) {
        fprintf(stderr, "ERROR: thread was null during de-register\n");
        _endthread();
    }
    
    /* Unlock to release any joins */
    pthread_mutex_unlock(__get_thread_running_mutex(myself));
    
    /* Wait until all "join" threads have copied our pointer */
    waiters_local = 128;
    while(waiters_local > 0) {
        pthread_mutex_lock(__get_thread_waiting_mutex(myself));
        waiters_local = __get_thread_waiters_count(myself);
        pthread_mutex_unlock(__get_thread_waiting_mutex(myself));
    }
    
    __unregister_thread(myself);
    
    /* This routine also needs to notify waiting threads */
    _endthread();
}    

