
#include "librv0_rwlock.h"

#ifndef librv0_rwlock_writelock_h
#define librv0_rwlock_writelock_h

//rwlock readlock
    typedef struct
    {
        pthread_rwlock_t    *rwl;
        librv0_rwlock       *prt;
    } librv0_rwlock_writelock;

#include "librv0_rwlock_ref.h"

//create new readlock
    librv0_rwlock_writelock *librv0_rwlock_writelock_create( librv0_rwlock *prt );
//destroy readlock
    void librv0_rwlock_writelock_destroy( librv0_rwlock_writelock **t );
//init readlock
    void __librv0_rwlock_writelock_init( librv0_rwlock_writelock *t, librv0_rwlock *prt );
//deinit readlock
    void __librv0_rwlock_writelock_deinit( librv0_rwlock_writelock *t );
//create new reference
    librv0_rwlock_ref *librv0_rwlock_writelock_create_ref( librv0_rwlock_writelock *t );

#endif // librv0_rwlock_readlock_h

