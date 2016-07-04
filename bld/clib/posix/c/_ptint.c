
#include "rterrno.h"
#include <pthread.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "_ptint.h"

static struct __ptcatalog_struct {
    pid_t     tid;
    pthread_t *pt;
    struct __ptcatalog_struct *next;
} *__ptcatalog;

static pthread_mutex_t *__ptcatalog_mutex;

pthread_t *__get_thread( pid_t tid )
{
pthread_t *ret;
struct __ptcatalog_struct *walker;

    ret = NULL;

    walker = __ptcatalog;
    while(walker != NULL) {
        if(walker->tid == tid) {
            ret = walker->pt;
            break;
        }
        walker = walker->next;
    }
    
    return ret;
}

pthread_t *__register_thread( )
{
struct __ptcatalog_struct *newthread;
struct __ptcatalog_struct *walker;

    newthread = (struct __ptcatalog_struct *)malloc(sizeof(struct __ptcatalog_struct));
    if( newthread == NULL ) {
        _RWD_errno = ENOMEM;
        return NULL;
    }
    
    newthread->tid = gettid();
    newthread->pt = (pthread_t *)malloc(sizeof(pthread_t));
    if( newthread->pt == NULL ) {
        free(newthread);
        _RWD_errno = ENOMEM;
        return NULL;
    }
    
    /* Initialize some aspects of our pthread object */
    newthread->pt->id = newthread->tid;
    newthread->pt->return_value = NULL;

    /* Check if we've ever created a thread */
    if(__ptcatalog_mutex == NULL) {
        __ptcatalog_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        if(__ptcatalog_mutex != NULL)
            printf("Mutex init = %d\n", pthread_mutex_init(__ptcatalog_mutex, NULL));
    }

    printf("Lock int list\n");
    
    /* Attach to the linked list */
    if(pthread_mutex_lock(__ptcatalog_mutex) == 0) {
        printf("Lock int list complete\n");
        if(__ptcatalog == NULL)
            __ptcatalog = newthread;
        else {
            walker = __ptcatalog;
            while(walker->next != NULL) walker = walker->next;
            walker->next = newthread;
        }
        printf("List update done\n");
        pthread_mutex_unlock(__ptcatalog_mutex);
    } else {
        _RWD_errno = EBUSY;
        return NULL;
    }
    
    return( newthread->pt );
}

static struct __ptcatalog_struct *__remove_thread( pid_t tid )
{
struct __ptcatalog_struct *walker, *previous;

    walker = NULL;

    if(pthread_mutex_lock(__ptcatalog_mutex) == 0) {

        walker = __ptcatalog;
        previous = NULL;
        while(walker != NULL) {
            if(walker->tid == tid) {
                break;
            }
            previous = walker;
            walker = walker->next;
        }
        
        /* Remove it from the linked list */
        if(walker != NULL && previous != NULL)
            previous = walker->next;
        else if(walker != NULL)
            __ptcatalog = walker->next;
        
        /* Remove its last link */
        if(walker != NULL)
            walker->next = NULL;
        
        pthread_mutex_unlock(__ptcatalog_mutex);
    }
    
    return walker;
}

void __unregister_thread( pthread_t *thread )
{
    if(thread != NULL) {
        __ptcatalog = __remove_thread(thread->id);
    }
}

void __unregister_current_thread( )
{
    __unregister_thread( __get_current_thread() );
}

pthread_t *__get_current_thread( )
{
#ifdef __LINUX__
    return( __get_thread(gettid()) );
#else
    return( NULL );
#endif
}
