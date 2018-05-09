
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
    //pointer to parent lock
        t->rwl = &prt->rwl;
    //create rwlock for local pointer
        pthread_rwlock_init( &t->rwl_prt, 0 );
    }

//deinit ref
    void __librv0_rwlock_ref_deinit( librv0_rwlock_ref *t )
    {
    //lock local pointer
        pthread_rwlock_wrlock( &t->rwl_prt );
        if( t->prt && t->rwl )
        {
        //lock parent
            pthread_rwlock_wrlock( t->rwl );
        //remove ptr from prt
            __librv0_rwlock_remove_ref( t->prt, t );
        //unlock parent
            pthread_rwlock_unlock( t->rwl );
        }
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //cleanup
        pthread_rwlock_destroy( &t->rwl_prt );
    }

//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock( librv0_rwlock_ref *t )
    {
        librv0_rwlock_readlock *r;
    //acquire lock on local pointer
        if( pthread_rwlock_rdlock( &t->rwl_prt ) )
            return 0;
    //if unlinked, unlock and return
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl_prt );
            return 0;
        }
    //create readlock
        r = librv0_rwlock_create_readlock( t->prt );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return readlock
        return r;
    }

//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_readlock *r;
    //convert time in ms to timespec
        librv0_rwlock_ms2ts( &ts, ms );
    //lock local pointer
        if( pthread_rwlock_timedrdlock( &t->rwl_prt, &ts ) )
            return 0;
    //if unlinked, unlock and return
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl_prt );
            return 0;
        }
    //create readlock
        r = librv0_rwlock_try_create_readlock( t->prt, ms );
    //unlock the local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return readlock
        return r;
    }

//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock( librv0_rwlock_ref *t )
    {
        librv0_rwlock_writelock *r;
    //lock local pointer
        if( pthread_rwlock_wrlock( &t->rwl_prt ) )
            return 0;
    //unlinked, unlock and return
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl_prt );
            return 0;
        }
    //create writelock
        r = librv0_rwlock_create_writelock( t->prt );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return struct
        return r;
    }

//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_writelock *r;
    //convert ms time to timepsec
        librv0_rwlock_ms2ts( &ts, ms );
    //lock local pointer
        if( pthread_rwlock_timedwrlock( &t->rwl_prt, &ts ) )
            return 0;
    //if unlinked, unlock and return
        if( !t->prt )
        {
            pthread_rwlock_unlock( &t->rwl_prt );
            return 0;
        }
    //create writelock
        r = librv0_rwlock_try_create_writelock( t->prt, ms );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return
        return r;
    }

#endif // librv0_rwlock_ref_h

