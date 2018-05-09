
#ifndef librv0_rwlock_c
#define librv0_rwlock_c

#include "librv0_rwlock.h"
#include "librv0_rwlock_readlock.h"
#include "librv0_rwlock_writelock.h"
#include "librv0_rwlock_ref.h"
#include <stdlib.h>
#include <string.h>

//convert ms to timespec
    void librv0_rwlock_ms2ts( struct timespec *ts, unsigned long long ms )
    {
        ts->tv_sec = ms / 1000;
        ts->tv_nsec = (ms % 1000) * 1000000;
    }

//create new rwlock
    librv0_rwlock *librv0_rwlock_create( void )
    {
        librv0_rwlock *r = malloc( sizeof( librv0_rwlock ) );
        if( r )
            __librv0_rwlock_init( r );
        return r;
    }

//destroy rwlock
    void librv0_rwlock_destroy( librv0_rwlock **t )
    {
        if( !t || !*t )
            return;
        __librv0_rwlock_deinit( *t );
        free( *t );
        *t = 0;
    }

//initiate rwlock
    void __librv0_rwlock_init( librv0_rwlock *t )
    {
        t->links.cnt = 0;
        t->links.ptr = 0;
        pthread_rwlock_init( &t->rwl, 0 );
    }

//deinit rwlock
    void __librv0_rwlock_deinit( librv0_rwlock *t )
    {
        int i;
        librv0_rwlock_ref_link *p;

        pthread_rwlock_wrlock( &t->rwl );

        for( i = 0; i < t->links.cnt; i++ )
        {
            p = &t->links.ptr[ i ];
            pthread_rwlock_wrlock( &p->rwl );
            if( p->ptr )
                *p->ptr = 0;
            pthread_rwlock_unlock( &p->rwl );
        }

        pthread_rwlock_unlock( &t->rwl );
        pthread_rwlock_destroy( &t->rwl );
    }

//attempt readlock, blocking until locked
    librv0_rwlock_readlock *librv0_rwlock_create_readlock( librv0_rwlock *t )
    {
        librv0_rwlock_readlock *r;
        if( pthread_rwlock_rdlock( &t->rwl ) )
            return 0;
        r = librv0_rwlock_readlock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_readlock *librv0_rwlock_try_create_readlock( librv0_rwlock *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_readlock *r;
        librv0_rwlock_ms2ts( &ts, ms );
        if( pthread_rwlock_timedrdlock( &t->rwl, &ts ) )
            return 0;
        r = librv0_rwlock_readlock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//attempt writelock, blocking until locked
    librv0_rwlock_writelock *librv0_rwlock_create_writelock( librv0_rwlock *t )
    {
        librv0_rwlock_writelock *r;
        if( pthread_rwlock_wrlock( &t->rwl ) )
            return 0;
        r = librv0_rwlock_writelock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_writelock *librv0_rwlock_try_create_writelock( librv0_rwlock *t, unsigned long long ms )
    {
        librv0_rwlock_writelock *r;
        struct timespec ts;
        librv0_rwlock_ms2ts( &ts, ms );
        if( pthread_rwlock_timedwrlock( &t->rwl, &ts ) )
            return 0;
        r = librv0_rwlock_writelock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
        return r;
    }

//find blank location on ref list and insert ref
    bool __librv0_rwlock_insert_reflist( librv0_rwlock *t, librv0_rwlock_ref *r )
    {
        int i;
        librv0_rwlock_ref_link *p;

        for( i = 0; i < t->links.cnt; i++ )
        {
            p = &t->links.ptr[ i ];
            if( p->ptr )
                continue;
            p->ptr = (void **)&r->prt;
            p->rwl = r->rwl_prt;
            r->rwl = t->rwl;
            return 1;
        }

        return 0;
    }

//create more space on ref list
    bool __librv0_rwlock_resize_reflist( librv0_rwlock *t )
    {
        unsigned int nsz, sz, ncnt;
        librv0_rwlock_ref_link *np;
        ncnt = t->links.cnt * 2;
        if( !ncnt )
            ncnt = 1;
        sz = t->links.cnt * sizeof( librv0_rwlock_ref_link );
        nsz = ncnt * sizeof( librv0_rwlock_ref_link );
        np = malloc( nsz );
        if( !np )
            return 0;
        memset( np, 0, nsz );
        if( sz && t->links.ptr )
            memcpy( np, t->links.ptr, sz );
        if( t->links.ptr )
            free( t->links.ptr );
        t->links.cnt = ncnt;
        t->links.ptr = np;
        return 1;
    }

#endif // librv0_rwlock_h

