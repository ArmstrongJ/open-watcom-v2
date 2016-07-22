
#include "variety.h"
#include <stdio.h>
#include <pthread.h>
#include <process.h>

#include "_ptint.h"

_WCRTLINK int pthread_join(pthread_t thread, void **value_ptr)
{
int res;

    /* Increment the thread's waiters */
    res = pthread_mutex_lock(thread.waiting_mutex);
    if(res != 0)
        return res;
    thread.waiters++;
    pthread_mutex_unlock(thread.waiting_mutex);

    /* Wait for the thread to release its running lock */
    res = pthread_mutex_lock(thread.running_mutex);
    if(res != 0)
        return res;
    
    /* Copy the "internal only" payload pointer */
    if(value_ptr != NULL) 
        *value_ptr = thread.return_value;

    /* Decrement the waiting count */
    res = pthread_mutex_lock(thread.waiting_mutex);
    if(res == 0) {
        thread.waiters--;
        pthread_mutex_unlock(thread.waiting_mutex);
    }
    
    /* Unlock it so others may use it */
    pthread_mutex_unlock(thread.running_mutex);
    
    return 0;
}    
