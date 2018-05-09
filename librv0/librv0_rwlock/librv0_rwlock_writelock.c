
#ifndef librv0_rwlock_writelock_c
#define librv0_rwlock_writelock_c

#include "librv0_rwlock_writelock.h"
#include <stdlib.h>

//create new writelock
    librv0_rwlock_writelock *librv0_rwlock_writelock_create( librv0_rwlock *prt )
    {
        librv0_rwlock_writelock *r;
        r = malloc( sizeof( librv0_rwlock_writelock ) );
        if( r )
            __librv0_rwlock_writelock_init( r, prt );
        return r;
    }

//destroy writelock
    void librv0_rwlock_writelock_destroy( librv0_rwlock_writelock **t )
    {
        if( !t || !*t )
            return;
        __librv0_rwlock_writelock_deinit( *t );
        free( *t );
        *t = 0;
    }

//init writelock
    void __librv0_rwlock_writelock_init( librv0_rwlock_writelock *t, librv0_rwlock *prt )
    {
        t->prt = prt;
        t->rwl = prt->rwl;
    }

//deinit writelock
    void __librv0_rwlock_writelock_deinit( librv0_rwlock_writelock *t )
    {
        pthread_rwlock_unlock( &t->rwl );
    }

#endif // librv0_rwlock_writelock_h


