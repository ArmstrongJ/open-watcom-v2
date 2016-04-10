
#include "rterrno.h"
#include <pthread.h>
#include <process.h>


static sem_t __ptcatalog_sem;

static struct __ptcatalog_struct {
    pid_t     tid;
    pthread_t *pt;
    struct __ptcatalog_struct *next;
} *__ptcatalog;

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

    newthread = (struct __ptcatalog_struct *)malloc(sizeof(struct __ptcatalog_struct));
    if( newthread == NULL ) {
        RWD_errno = ENOMEM;
        return NULL;
    }
    
    newthread->tid = gettid();
    newthread->pt = (pthread_t *)malloc(sizeof(pthread_t));
    if( newthread->pt == NULL ) {
        free(newthread);
        RWD_errno = ENOMEM;
        return NULL;
    }
    
    /* Attach to the linked list */
    
    
}

pthread_t *__get_current_thread( )
{
#ifdef __LINUX__
    return( __get_thread(gettid()) );
#else
    return( NULL );
#endif
}
