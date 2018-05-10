
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
        librv0_rwlock *r;
    //allocate memory for struct
        r = malloc( sizeof( librv0_rwlock ) );
    //init struct
        if( r )
            __librv0_rwlock_init( r );
    //return struct
        return r;
    }

//destroy rwlock
    void librv0_rwlock_destroy( librv0_rwlock **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        __librv0_rwlock_deinit( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//initiate rwlock
    void __librv0_rwlock_init( librv0_rwlock *t )
    {
    //zero out ref list
        t->links.cnt = 0;
        t->links.ptr = 0;
    //create pthread rwlock
        pthread_rwlock_init( &t->rwl, 0 );
    }

//deinit rwlock
    void __librv0_rwlock_deinit( librv0_rwlock *t )
    {
        int i;
        librv0_rwlock_ref_link *p;
    //write lock pthread rwlock
        pthread_rwlock_wrlock( &t->rwl );
    //for every ref, zero out pointer to this struct
        for( i = 0; i < t->links.cnt; i++ )
        {
            p = &t->links.ptr[ i ];
            if( !p->rwl || !p->ptr )
                continue;
        //wrap pointer zeroing in write locks
            pthread_rwlock_wrlock( p->rwl );
        //zero pointer
            *p->ptr = 0;
        //unlock
            pthread_rwlock_unlock( p->rwl );
        }
    //unlock and destroy rwlock
        pthread_rwlock_unlock( &t->rwl );
        pthread_rwlock_destroy( &t->rwl );
    }

//attempt readlock, blocking until locked
    librv0_rwlock_readlock *librv0_rwlock_create_readlock( librv0_rwlock *t )
    {
        librv0_rwlock_readlock *r;
    //attempt to acquire readlock
        if( pthread_rwlock_rdlock( &t->rwl ) )
            return 0;
    //create readlock struct or unlock on fail
        r = librv0_rwlock_readlock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
    //return struct
        return r;
    }

//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_readlock *librv0_rwlock_try_create_readlock( librv0_rwlock *t, unsigned long long ms )
    {
        struct timespec ts;
        librv0_rwlock_readlock *r;
    //convert ms time into timespec
        librv0_rwlock_ms2ts( &ts, ms );
    //acquire readlock
        if( pthread_rwlock_timedrdlock( &t->rwl, &ts ) )
            return 0;
    //create readlock struct or release lock on fail
        r = librv0_rwlock_readlock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
    //return struct
        return r;
    }

//attempt writelock, blocking until locked
    librv0_rwlock_writelock *librv0_rwlock_create_writelock( librv0_rwlock *t )
    {
        librv0_rwlock_writelock *r;
    //acquire writelock
        if( pthread_rwlock_wrlock( &t->rwl ) )
            return 0;
    //create writelock struct or unlock on fail
        r = librv0_rwlock_writelock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
    //return struct
        return r;
    }

//attempt readlock, blocking until locked or ms time passes
    librv0_rwlock_writelock *librv0_rwlock_try_create_writelock( librv0_rwlock *t, unsigned long long ms )
    {
        librv0_rwlock_writelock *r;
        struct timespec ts;
    //convert ms time into timespec
        librv0_rwlock_ms2ts( &ts, ms );
    //acquire writelock
        if( pthread_rwlock_timedwrlock( &t->rwl, &ts ) )
            return 0;
    //create writelock struct or unlock on fail
        r = librv0_rwlock_writelock_create( t );
        if( !r )
            pthread_rwlock_unlock( &t->rwl );
    //return struct
        return r;
    }

//find blank location on ref list and insert ref
    bool __librv0_rwlock_insert_reflist( librv0_rwlock *t, librv0_rwlock_ref *r )
    {
        int i;
        librv0_rwlock_ref_link *p;
    //search for zeroed link
        for( i = 0; i < t->links.cnt; i++ )
        {
            p = &t->links.ptr[ i ];
        //not zeroed, keep looking
            if( p->ptr )
                continue;
        //set pointers
            p->ptr = (void **)&r->prt;
            p->rwl = &r->rwl_prt;
            r->rwl = &t->rwl;
        //return sucess
            return 1;
        }
    //return failure
        return 0;
    }

//create more space on ref list
    bool __librv0_rwlock_resize_reflist( librv0_rwlock *t )
    {
        unsigned int nsz, sz, ncnt;
        librv0_rwlock_ref_link *np;
    //new size will be double old size
        ncnt = t->links.cnt * 2;
    //0 size, fix
        if( !ncnt )
            ncnt = 1;
    //convert counts into byte sizes
        sz = t->links.cnt * sizeof( librv0_rwlock_ref_link );
        nsz = ncnt * sizeof( librv0_rwlock_ref_link );
    //allocate new list
        np = malloc( nsz );
        if( !np )
            return 0;
    //copy over old list
        memset( np, 0, nsz );
        if( sz && t->links.ptr )
            memcpy( np, t->links.ptr, sz );
    //release old memory
        if( t->links.ptr )
            free( t->links.ptr );
    //set pointers to new list
        t->links.cnt = ncnt;
        t->links.ptr = np;
    //return sucess
        return 1;
    }

//remove ref from list
    void __librv0_rwlock_remove_ref( librv0_rwlock *t, librv0_rwlock_ref *r )
    {
        int i;
        librv0_rwlock_ref_link *p;
    //search for link
        for( i = 0; i < t->links.cnt; i++ )
        {
            p = &t->links.ptr[ i ];
        //keep looking
            if( p->rwl != &r->rwl_prt )
                continue;
        //set pointers
            p->ptr = 0;
            p->rwl = 0;
        }
    }

//test rwlock lib
    bool librv0_rwlock_test( void )
    {
        bool b;
        librv0_rwlock *rwl;
        librv0_rwlock_readlock *rl0, *rl1, *rl2, *rl3;
        librv0_rwlock_writelock *wl0, *wl1;
        librv0_rwlock_ref *r0, *r1, *r2, *r3;

    //create and destroy rwlock object
        rwl = librv0_rwlock_create();
        if( !rwl )
            return 0;
        librv0_rwlock_destroy( &rwl );
        rwl = librv0_rwlock_create();
        if( !rwl )
            return 0;

    //attempt multiple readlocks
        rl0 = librv0_rwlock_create_readlock( rwl );
        b = rl0 != 0;
        rl1 = librv0_rwlock_try_create_readlock( rwl, 100 );
        b &= rl1 != 0;
        rl2 = librv0_rwlock_try_create_readlock( rwl, 100 );
        b &= rl2 != 0;
        rl3 = librv0_rwlock_try_create_readlock( rwl, 100 );
        b &= rl3 != 0;
        librv0_rwlock_readlock_destroy( &rl0 );
        librv0_rwlock_readlock_destroy( &rl1 );
        librv0_rwlock_readlock_destroy( &rl2 );
        librv0_rwlock_readlock_destroy( &rl3 );
        if( !b )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //attempt multiple writelocks
        wl0 = librv0_rwlock_create_writelock( rwl );
        if( !wl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        wl1 = librv0_rwlock_try_create_writelock( rwl, 100 );
        librv0_rwlock_writelock_destroy( &wl0 );
        if( wl1 )
        {
            librv0_rwlock_writelock_destroy( &wl1 );
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //attempt writelock during readlock
        rl0 = librv0_rwlock_create_readlock( rwl );
        if( !rl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        wl0 = librv0_rwlock_try_create_writelock( rwl, 100 );
        librv0_rwlock_readlock_destroy( &rl0 );
        if( wl0 )
        {
            librv0_rwlock_writelock_destroy( &wl0 );
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //attempt readlock during writelock
        wl0 = librv0_rwlock_create_writelock( rwl );
        if( !wl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        rl0 = librv0_rwlock_try_create_readlock( rwl, 100 );
        librv0_rwlock_writelock_destroy( &wl0 );
        if( rl0 )
        {
            librv0_rwlock_readlock_destroy( &rl0 );
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //test creating and deleting refs
        wl0 = librv0_rwlock_create_writelock( rwl );
        if( !wl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        r0 = librv0_rwlock_writelock_create_ref( wl0 );
        b = r0 != 0;
        r1 = librv0_rwlock_writelock_create_ref( wl0 );
        b &= r1 != 0;
        r2 = librv0_rwlock_writelock_create_ref( wl0 );
        b &= r2 != 0;
        r3 = librv0_rwlock_writelock_create_ref( wl0 );
        b &= r3 != 0;
        librv0_rwlock_writelock_destroy( &wl0 );
        librv0_rwlock_ref_destroy( &r0 );
        librv0_rwlock_ref_destroy( &r1 );
        librv0_rwlock_ref_destroy( &r2 );
        librv0_rwlock_ref_destroy( &r3 );
        if( !b )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //test writelock during readlock
        wl0 = librv0_rwlock_create_writelock( rwl );
        if( !wl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        r0 = librv0_rwlock_writelock_create_ref( wl0 );
        b = r0 != 0;
        r1 = librv0_rwlock_writelock_create_ref( wl0 );
        b &= r1 != 0;
        if( !b )
        {
            librv0_rwlock_ref_destroy( &r0 );
            librv0_rwlock_ref_destroy( &r1 );
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        librv0_rwlock_writelock_destroy( &wl0 );
        rl0 = librv0_rwlock_ref_try_create_readlock( r0, 100 );
        b = rl0 != 0;
        wl0 = librv0_rwlock_ref_try_create_writelock( r1, 100 );
        b &= wl0 == 0;
        librv0_rwlock_readlock_destroy( &rl0 );
        librv0_rwlock_writelock_destroy( &wl0 );
        librv0_rwlock_ref_destroy( &r0 );
        librv0_rwlock_ref_destroy( &r1 );
        if( !b )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }

    //destroy rwlock object and then attempt to use refs
        wl0 = librv0_rwlock_create_writelock( rwl );
        if( !wl0 )
        {
            librv0_rwlock_destroy( &rwl );
            return 0;
        }
        r0 = librv0_rwlock_writelock_create_ref( wl0 );
        b = r0 != 0;
        r1 = librv0_rwlock_writelock_create_ref( wl0 );
        b &= r1 != 0;
        librv0_rwlock_writelock_destroy( &wl0 );
        librv0_rwlock_destroy( &rwl );
        if( !b )
        {
            librv0_rwlock_ref_destroy( &r0 );
            librv0_rwlock_ref_destroy( &r1 );
            return 0;
        }
        rl0 = librv0_rwlock_ref_try_create_readlock( r0, 100 );
        b = rl0 == 0;
        wl0 = librv0_rwlock_ref_try_create_writelock( r1, 100 );
        b &= wl0 == 0;
        librv0_rwlock_readlock_destroy( &rl0 );
        librv0_rwlock_writelock_destroy( &wl0 );
        librv0_rwlock_ref_destroy( &r0 );
        librv0_rwlock_ref_destroy( &r1 );
        if( !b )
        {
            return 0;
        }

        return 1;
    }

#endif // librv0_rwlock_h

