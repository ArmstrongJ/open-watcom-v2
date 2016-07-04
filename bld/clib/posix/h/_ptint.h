#ifndef _PT_INTERNAL_H_INCLUDED
#define _PT_INTERNAL_H_INCLUDED

#include <sys/types.h>

pthread_t *__register_thread();
pthread_t *__get_thread( pid_t tid );
pthread_t *__get_current_thread( );
void __unregister_thread( pthread_t *thread );
void __unregister_current_thread( );

#endif /* _PT_INTERNAL_H_INCLUDED */
