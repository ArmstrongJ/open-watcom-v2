
#include "variety.h"
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include "atomic.h"

#define SL_LOCKED     0
#define SL_UNLOCKED   1

_WCRTLINK int pthread_spin_init(pthread_spinlock_t *__lock, int __ignored_pshared)
{
    if(__lock == NULL) 
        return( EINVAL );
    
    __lock->value = (int *)malloc(sizeof(int));
    if(__lock->value == NULL)
        return( ENOMEM );
        
    *__lock->value = SL_UNLOCKED;
    return( 0 );
}

_WCRTLINK int pthread_spin_destroy(pthread_spinlock_t *__lock)
{
    if(__lock == NULL)
        return( EINVAL );

    if(pthread_spin_trylock(__lock) != 0)
        return( EBUSY );
    
    free(__lock->value);
    return( 0 );
}

_WCRTLINK int pthread_spin_lock(pthread_spinlock_t *__lock)
{
    if(__lock == NULL)
        return( EINVAL );

    /* spin away */
    while(pthread_spin_trylock(__lock) != 0);
    
    return( 0 );
}

_WCRTLINK int pthread_spin_trylock(pthread_spinlock_t *__lock)
{
    if(__lock == NULL)
        return( EINVAL );
        
    if(__atomic_compare_and_swap(__lock->value, SL_LOCKED, SL_UNLOCKED))
        return( 0 );
        
    return( EBUSY );
}

_WCRTLINK int pthread_spin_unlock(pthread_spinlock_t *__lock)
{
    if(__lock == NULL)
        return( EINVAL );
        
    __atomic_compare_and_swap(__lock->value, SL_UNLOCKED, SL_LOCKED);
    
    return( 0 );
}
