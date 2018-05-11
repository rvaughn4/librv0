
#ifndef librv0_rwlock_readlock_c
#define librv0_rwlock_readlock_c

#include "librv0_rwlock_readlock.h"
#include <stdlib.h>

//create new readlock
    librv0_rwlock_readlock *librv0_rwlock_readlock_create( librv0_rwlock *prt )
    {
        librv0_rwlock_readlock *r;
    //test pointer
        if( !prt )
            return 0;
    //allocate memory
        r = malloc( sizeof( librv0_rwlock_readlock ) );
    //init struct
        librv0_rwlock_readlock_create_on_stack( prt, r );
    //return
        return r;
    }

//create new readlock on stack
    librv0_rwlock_readlock *librv0_rwlock_readlock_create_on_stack( librv0_rwlock *prt, librv0_rwlock_readlock *t )
    {
    //test pointers
        if( !t || !prt )
            return 0;
    //init struct
        __librv0_rwlock_readlock_init( t, prt );
    //return
        return t;
    }

//destroy readlock
    void librv0_rwlock_readlock_destroy( librv0_rwlock_readlock **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        librv0_rwlock_readlock_destroy_on_stack( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//destroy readlock
    void librv0_rwlock_readlock_destroy_on_stack( librv0_rwlock_readlock *t )
    {
    //test pointers
        if( !t )
            return;
    //deinit struct
        __librv0_rwlock_readlock_deinit( t );
    }

//init readlock
    void __librv0_rwlock_readlock_init( librv0_rwlock_readlock *t, librv0_rwlock *prt )
    {
    //set pointers
        t->prt = prt;
        t->rwl = &prt->rwl;
    }

//deinit readlock
    void __librv0_rwlock_readlock_deinit( librv0_rwlock_readlock *t )
    {
    //unlock rwlock struct
        pthread_rwlock_unlock( t->rwl );
    }

#endif // librv0_rwlock_readlock_h

