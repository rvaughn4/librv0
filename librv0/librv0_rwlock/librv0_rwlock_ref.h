
#include "librv0_rwlock.h"

#ifndef librv0_rwlock_ref_h
#define librv0_rwlock_ref_h

#include <pthread.h>
#include <stdlib.h>

//rwlock reference
    typedef struct
    {
        librv0_rwlock *prt;
        pthread_rwlock_t *rwl, rwl_prt;
    } librv0_rwlock_ref;

#include "librv0_rwlock_readlock.h"
#include "librv0_rwlock_writelock.h"

//create new ref
    librv0_rwlock_ref *librv0_rwlock_ref_create( librv0_rwlock *prt, librv0_rwlock_writelock *l );
//destroy ref
    void librv0_rwlock_ref_destroy( librv0_rwlock_ref **t );
//init ref
    void __librv0_rwlock_ref_init( librv0_rwlock_ref *t, librv0_rwlock *prt, librv0_rwlock_writelock *l );
//deinit ref
    void __librv0_rwlock_ref_deinit( librv0_rwlock_ref *t );
//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock( librv0_rwlock_ref *t );
//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock( librv0_rwlock_ref *t, unsigned long long ms );
//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock( librv0_rwlock_ref *t );
//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock( librv0_rwlock_ref *t, unsigned long long ms );

#endif // librv0_rwlock_ref_h