
#ifndef librv0_rwlock_ref_c
#define librv0_rwlock_ref_c

#include "librv0_rwlock_ref.h"
#include <pthread.h>

//create new ref
    librv0_rwlock_ref *librv0_rwlock_ref_create( librv0_rwlock *prt, librv0_rwlock_writelock *l )
    {
        librv0_rwlock_ref *r;
     //allocate memory
        r = malloc( sizeof( librv0_rwlock_ref ) );
    //init struct
        librv0_rwlock_ref_create_on_stack( prt, l, r );
    //return
        return r;
    }

//create new ref on the stack
    librv0_rwlock_ref *librv0_rwlock_ref_create_on_stack( librv0_rwlock *prt, librv0_rwlock_writelock *l, librv0_rwlock_ref *t )
    {
    //test pointers
        if( !t || !l || !prt )
            return 0;
    //init struct
        __librv0_rwlock_ref_init( t, prt, l );
    //return
        return t;
    }

//destroy ref
    void librv0_rwlock_ref_destroy( librv0_rwlock_ref **t )
    {
    //testpointer for null
        if( !t || !*t )
            return;
    //deinit struct
        librv0_rwlock_ref_destroy_on_stack( *t );
    //release memory
        free( *t );
    //set to null
        *t = 0;
    }

//destroy ref on the stack
    void librv0_rwlock_ref_destroy_on_stack( librv0_rwlock_ref *t )
    {
    //testpointer for null
        if( !t )
            return;
    //deinit struct
        __librv0_rwlock_ref_deinit( t );
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
        return librv0_rwlock_ref_create_readlock_on_stack( t, 0 );
    }

//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        return librv0_rwlock_ref_try_create_readlock_on_stack( t, 0, ms );
    }

//create readlock
    librv0_rwlock_readlock *librv0_rwlock_ref_create_readlock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_readlock *l )
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
        if( !l )
            r = librv0_rwlock_create_readlock( t->prt );
        else
            r = librv0_rwlock_create_readlock_on_stack( t->prt, l );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return readlock
        return r;
    }

//create readlock with timeout
    librv0_rwlock_readlock *librv0_rwlock_ref_try_create_readlock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_readlock *l, unsigned long long ms )
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
        if( !l )
            r = librv0_rwlock_try_create_readlock( t->prt, ms );
        else
            r = librv0_rwlock_try_create_readlock_on_stack( t->prt, l, ms );
    //unlock the local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return readlock
        return r;
    }

//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock( librv0_rwlock_ref *t )
    {
        return librv0_rwlock_ref_create_writelock_on_stack( t, 0 );
    }

//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock( librv0_rwlock_ref *t, unsigned long long ms )
    {
        return librv0_rwlock_ref_try_create_writelock_on_stack( t, 0, ms );
    }

//create writelock
    librv0_rwlock_writelock *librv0_rwlock_ref_create_writelock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_writelock *l )
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
        if( !l )
            r = librv0_rwlock_create_writelock( t->prt );
        else
            r = librv0_rwlock_create_writelock_on_stack( t->prt, l );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return struct
        return r;
    }

//create writelock with timeout
    librv0_rwlock_writelock *librv0_rwlock_ref_try_create_writelock_on_stack( librv0_rwlock_ref *t, librv0_rwlock_writelock *l, unsigned long long ms )
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
        if( !l )
            r = librv0_rwlock_try_create_writelock( t->prt, ms );
        else
            r = librv0_rwlock_try_create_writelock_on_stack( t->prt, l, ms );
    //unlock local pointer
        pthread_rwlock_unlock( &t->rwl_prt );
    //return
        return r;
    }

#endif // librv0_rwlock_ref_h

