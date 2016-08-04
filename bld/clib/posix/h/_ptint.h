#ifndef _PT_INTERNAL_H_INCLUDED
#define _PT_INTERNAL_H_INCLUDED

#include <sys/types.h>

/* Thread bookkeeping */
pthread_t *__register_thread();
pthread_t *__get_thread( pid_t tid );
pthread_t *__get_current_thread( );
void __unregister_thread( pthread_t *thread );
void __unregister_current_thread( );

/* Thread-specific keys bookkeeping */
pthread_key_t __register_pkey( void (*destructor)(void*) );
void __destroy_pkey( pthread_key_t id );
int __valid_pkey_id( pthread_key_t id );
int __set_pkey_value( pthread_key_t id, void *value );
void *__get_pkey_value( pthread_key_t id );

#endif /* _PT_INTERNAL_H_INCLUDED */
