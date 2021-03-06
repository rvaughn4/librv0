
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
        void *ext_ptr;
    } librv0_rwlock_ref;

#include "librv0_rwlock_readlock.h"
#include "librv0_rwlock_writelock.h"

//create new ref
    librv0_rwlock_ref *librv0_rwlock_ref_create( librv0_rwlock *prt, librv0_rwlock_writelock *l, void *ext_ptr );
//create new ref on the stack
    librv0_rwlock_ref *librv0_rwlock_ref_create_on_stack( librv0_rwlock *prt, librv0_rwlock_writelock *l, librv0_rwlock_ref *t, void *ext_ptr );
//destroy ref
    void librv0_rwlock_ref_destroy( librv0_rwlock_ref **t );
//destroy ref on the stack
    void librv0_rwlock_ref_destroy_on_stack( librv0_rwlock_ref *t );
//init ref
    void __librv0_rwlock_ref_init( librv0_rwlock_ref *t, librv0_rwlock *prt, librv0_rwlock_writelock *l, void *ext_ptr );
//deinit ref
    void __librv0_rwlock_ref_deinit( librv0_rwlock_ref *t );
//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock( librv0_rwlock_ref *t );
//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock( librv0_rwlock_ref *t, unsigned long long ms );
//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_readlock *l );
//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_readlock *l, unsigned long long ms );
//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock( librv0_rwlock_ref *t );
//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock( librv0_rwlock_ref *t, unsigned long long ms );
//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_writelock *l );
//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_writelock *l, unsigned long long ms );
//lock external pointer
    void *librv0_rwlock_ref_lock_ext_pointer( librv0_rwlock_ref *t );
//lock external pointer
    void *librv0_rwlock_ref_try_lock_ext_pointer( librv0_rwlock_ref *t, unsigned long long ms );
//unlock external pointer
    void librv0_rwlock_ref_unlock_ext_pointer( librv0_rwlock_ref *t );

#endif // librv0_rwlock_ref_h
