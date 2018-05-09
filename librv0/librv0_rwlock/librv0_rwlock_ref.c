
#ifndef librv0_rwlock_ref_c
#define librv0_rwlock_ref_c

#include "librv0_rwlock_ref.h"
#include <pthread.h>

//create new ref
    librv0_rwlock_ref *librv0_rwlock_ref_create( librv0_rwlock *prt, librv0_rwlock_writelock *l )
    {
        librv0_rwlock_ref *r;
        r = malloc( sizeof( librv0_rwlock_ref ) );
        if( r )
            __librv0_rwlock_ref_init( r, prt, l );
        return r;
    }

//destroy ref
    void librv0_rwlock_ref_destroy( librv0_rwlock_ref **t )
    {
        if( !t || !*t )
            return;
        __librv0_rwlock_ref_deinit( *t );
        *t = 0;
    }

//init ref
    void __librv0_rwlock_ref_init( librv0_rwlock_ref *t, librv0_rwlock *prt, librv0_rwlock_writelock *l )
    {
        t->prt = prt;
        t->rwl_prt = prt->rwl;
        pthread_rwlock_init( &t->rwl, 0 );
    }

//deinit ref
    void __librv0_rwlock_ref_deinit( librv0_rwlock_ref *t )
    {
        pthread_rwlock_wrlock( &t->rwl );
        pthread_rwlock_wrlock( &t->rwl_prt );
        //remove ptr from prt
        pthread_rwlock_unlock( &t->rwl_prt );
        pthread_rwlock_unlock( &t->rwl );
        pthread_rwlock_destroy( &t->rwl );
    }

//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock( librv0_rwlock_ref *t )
    {
        librv0_rwlock_readlock *r;
        if( pthread_rwlock_rdlock( &t->rwl ) )
            return 0;
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl );
            return 0;
        }
        r = librv0_rwlock_create_readlock( t->prt );
        pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_readlock *r;
        librv0_rwlock_ms2ts( &ts, ms );
        if( pthread_rwlock_timedrdlock( &t->rwl, &ts ) )
            return 0;
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl );
            return 0;
        }
        r = librv0_rwlock_try_create_readlock( t->prt, ms );
        pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock( librv0_rwlock_ref *t )
    {
        librv0_rwlock_writelock *r;
        if( pthread_rwlock_wrlock( &t->rwl ) )
            return 0;
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl );
            return 0;
        }
        r = librv0_rwlock_create_writelock( t->prt );
        pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_writelock *r;
        librv0_rwlock_ms2ts( &ts, ms );
        if( pthread_rwlock_timedwrlock( &t->rwl, &ts ) )
            return 0;
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl );
            return 0;
        }
        r = librv0_rwlock_try_create_writelock( t->prt, ms );
        pthread_rwlock_unlock( &t->rwl );
        return r;
    }

#endif // librv0_rwlock_ref_h

