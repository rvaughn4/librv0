
#ifndef librv0_rwlock_h
#define librv0_rwlock_h

#include <pthread.h>
#include <stdbool.h>

//link to refs
    typedef struct
    {
        pthread_rwlock_t    *rwl;
        void                **ptr;
    } librv0_rwlock_ref_link;

//rw lock
    typedef struct
    {
        pthread_rwlock_t rwl;
        struct
        {
            librv0_rwlock_ref_link *ptr;
            unsigned int cnt;
        } links;
    } librv0_rwlock;

#include "librv0_rwlock_readlock.h"
#include "librv0_rwlock_writelock.h"
#include "librv0_rwlock_ref.h"

//convert ms to timespec
    void librv0_rwlock_ms2ts( struct timespec *ts, unsigned long long ms );
//create new rwlock
    librv0_rwlock *librv0_rwlock_create( void );
//create new rwlock on stack
    librv0_rwlock *librv0_rwlock_create_on_stack( librv0_rwlock *t );
//destroy rwlock
    void librv0_rwlock_destroy( librv0_rwlock **t );
//destroy rwlock on stack
    void librv0_rwlock_destroy_on_stack( librv0_rwlock *t );
//initiate rwlock
    void __librv0_rwlock_init( librv0_rwlock *t );
//deinit rwlock
    void __librv0_rwlock_deinit( librv0_rwlock *t );
//attempt readlock, blocking until locked
    librv0_rwlock_readlock *librv0_rwlock_create_readlock( librv0_rwlock *t );
//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_readlock *librv0_rwlock_try_create_readlock( librv0_rwlock *t, unsigned long long ms );
//attempt readlock, blocking until locked
    librv0_rwlock_readlock *librv0_rwlock_create_readlock_on_stack( librv0_rwlock *t, librv0_rwlock_readlock *l );
//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_readlock *librv0_rwlock_try_create_readlock_on_stack( librv0_rwlock *t, librv0_rwlock_readlock *l, unsigned long long ms );
//attempt writelock, blocking until locked
    librv0_rwlock_writelock *librv0_rwlock_create_writelock( librv0_rwlock *t );
//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_writelock *librv0_rwlock_try_create_writelock( librv0_rwlock *t, unsigned long long ms );
//attempt writelock, blocking until locked
    librv0_rwlock_writelock *librv0_rwlock_create_writelock_on_stack( librv0_rwlock *t, librv0_rwlock_writelock *l );
//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_writelock *librv0_rwlock_try_create_writelock_on_stack( librv0_rwlock *t, librv0_rwlock_writelock *l, unsigned long long ms );
//find blank location on ref list and insert ref
    bool __librv0_rwlock_insert_reflist( librv0_rwlock *t, librv0_rwlock_ref *r );
//create more space on ref list
    bool __librv0_rwlock_resize_reflist( librv0_rwlock *t );
//remove ref from list
    void __librv0_rwlock_remove_ref( librv0_rwlock *t, librv0_rwlock_ref *r );
//test rwlock lib
    bool librv0_rwlock_test( void );

#endif // librv0_rwlock_h
