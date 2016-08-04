
#include "variety.h"
#include <sys/types.h>
#include "rterrno.h"
#include "_ptint.h"

_WCRTLINK int pthread_key_create( pthread_key_t *__key, void (*__destructor)(void*) )
{
int ret;
pthread_key_t res;

    res = __register_pkey(__destructor);
    if(res < 0) 
        ret = errno;
    else {
        ret = 0;
        *__key = res;
    }
    return( ret );   
}

_WCRTLINK int pthread_key_delete(pthread_key_t __key)
{
int ret;

    ret = 0;

    if(__valid_pkey_id(__key) == 0) 
        __destroy_pkey(__key);
    else
        ret = EINVAL;
        
    return( ret );
}

_WCRTLINK int pthread_setspecific(pthread_key_t __key, void *__value)
{
int ret;

    if(__valid_pkey_id(__key) == 0) 
        ret = __set_pkey_value(__key, __value);
    else
        ret = EINVAL;
    
    return( ret );
}

_WCRTLINK void *pthread_getspecific(pthread_key_t __key)
{
    return( __get_pkey_value(__key) );
}
