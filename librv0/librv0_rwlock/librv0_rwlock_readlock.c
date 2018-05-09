
#ifndef librv0_rwlock_readlock_c
#define librv0_rwlock_readlock_c

#include "librv0_rwlock_readlock.h"
#include <stdlib.h>

//create new readlock
    librv0_rwlock_readlock *librv0_rwlock_readlock_create( librv0_rwlock *prt )
    {
        librv0_rwlock_readlock *r;
        r = malloc( sizeof( librv0_rwlock_readlock ) );
        if( r )
            librv0_rwlock_readlock_init( r, prt );
        return r;
    }

//destroy readlock
    void librv0_rwlock_readlock_destroy( librv0_rwlock_readlock **t )
    {
        if( !t || !*t )
            return;
        librv0_rwlock_readlock_deinit( *t );
        free( *t );
        *t = 0;
    }

//init readlock
    void librv0_rwlock_readlock_init( librv0_rwlock_readlock *t, librv0_rwlock *prt )
    {
        t->prt = prt;
        t->rwl = prt->rwl;
    }

//deinit readlock
    void librv0_rwlock_readlock_deinit( librv0_rwlock_readlock *t )
    {
        pthread_rwlock_unlock( &t->rwl );
    }

#endif // librv0_rwlock_readlock_h

