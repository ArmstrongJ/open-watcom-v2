
#include "variety.h"
#include <pthread.h>
#include "_ptint.h"

_WCRTLINK void pthread_cleanup_push( void (*__routine)(void*), void *__arg )
{
    __push_pthread_cleaner( __routine, __arg);
}

_WCRTLINK void pthread_cleanup_pop( int __execute )
{
    __pop_pthread_cleaner( __execute );
}
