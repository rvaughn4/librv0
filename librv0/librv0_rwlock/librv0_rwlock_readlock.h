
#include "librv0_rwlock.h"


#ifndef librv0_rwlock_readlock_h
#define librv0_rwlock_readlock_h

//rwlock readlock
    typedef struct
    {
        pthread_rwlock_t    *rwl;
        librv0_rwlock       *prt;
    } librv0_rwlock_readlock;

//create new readlock
    librv0_rwlock_readlock *librv0_rwlock_readlock_create( librv0_rwlock *prt );
//create new readlock on stack
    librv0_rwlock_readlock *librv0_rwlock_readlock_create_on_stack( librv0_rwlock *prt, librv0_rwlock_readlock *t );
//destroy readlock
    void librv0_rwlock_readlock_destroy( librv0_rwlock_readlock **t );
//destroy readlock on stack
    void librv0_rwlock_readlock_destroy_on_stack( librv0_rwlock_readlock *t );
//init readlock
    void __librv0_rwlock_readlock_init( librv0_rwlock_readlock *t, librv0_rwlock *prt );
//deinit readlock
    void __librv0_rwlock_readlock_deinit( librv0_rwlock_readlock *t );

#endif // librv0_rwlock_readlock_h
