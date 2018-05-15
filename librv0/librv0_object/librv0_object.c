
#ifndef librv0_object_c
#define librv0_object_c

#include "librv0_object.h"
#include "librv0_object_readlock.h"
#include "librv0_object_writelock.h"
#include "librv0_object_ref.h"
#include <string.h>

//create new object
    librv0_object *librv0_object_create( void )
    {
        librv0_object *r;
    //allocate memory
        r = malloc( sizeof( librv0_object ) );
    //init struct
        if( r )
            __librv0_object_init( r );
    //return
        return r;
    }

//destroy object
    void librv0_object_destroy( librv0_object **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        (* ( ( __librv0_object_deinit_ptr )( *t )->func_deinit ) )( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//init struct
    void __librv0_object_init( librv0_object *t )
    {
        t->bDoRun = 1;
        t->bHasStart = 0;
        t->delayTicks = 100;
        t->lastEpoch = 0;
        t->lastTicks = 0;
        t->priority = 0;
        t->id = 0;
        librv0_rwlock_create_on_stack( &t->rwl );
        __librv0_object_set_gen_readlock_func( t, __librv0_object_gen_readlock );
        __librv0_object_set_gen_writelock_func( t, __librv0_object_gen_writelock );
        __librv0_object_set_execute_func( t, __librv0_object_execute );
        __librv0_object_set_deinit_func( t, __librv0_object_deinit );
    }

//deinit struct
    void __librv0_object_deinit( librv0_object *t )
    {
        librv0_rwlock_destroy_on_stack( &t->rwl );
    }

//create readlock
    librv0_object_readlock *librv0_object_create_readlock( librv0_object *t )
    {
        librv0_object_readlock *r;
        librv0_rwlock_readlock rl;
    //attempt readlock acquire
        if( !librv0_rwlock_create_readlock_on_stack( &t->rwl, &rl ) )
            return 0;
    //generate object readlock
        r = ( *( ( __librv0_object_gen_readlock_ptr )( t->func_gen_readlock ) ) )( t );
        if( r )
        {
            memcpy( &r->rwl, &rl, sizeof( librv0_rwlock_readlock ) );
            return r;
        }
    //cleanup readlock
        librv0_rwlock_readlock_destroy_on_stack( &rl );
    //return null
        return 0;
    }

//create readlock with timeout
    librv0_object_readlock *librv0_object_try_create_readlock( librv0_object *t, unsigned long long ms )
    {
        librv0_object_readlock *r;
        librv0_rwlock_readlock rl;
    //attempt readlock acquire
        if( !librv0_rwlock_try_create_readlock_on_stack( &t->rwl, &rl, ms ) )
            return 0;
    //generate object readlock
        r = ( *( ( __librv0_object_gen_readlock_ptr )( t->func_gen_readlock ) ) )( t );
        if( r )
        {
            memcpy( &r->rwl, &rl, sizeof( librv0_rwlock_readlock ) );
            return r;
        }
    //cleanup readlock
        librv0_rwlock_readlock_destroy_on_stack( &rl );
    //return null
        return 0;
    }

//create writelock
    librv0_object_writelock *librv0_object_create_writelock( librv0_object *t )
    {
        librv0_object_writelock *r;
        librv0_rwlock_writelock rl;
    //attempt writelock acquire
        if( !librv0_rwlock_create_writelock_on_stack( &t->rwl, &rl ) )
            return 0;
    //generate object writelock
        r = ( *( ( __librv0_object_gen_writelock_ptr )( t->func_gen_writelock ) ) )( t );
        if( r )
        {
            memcpy( &r->rwl, &rl, sizeof( librv0_rwlock_writelock ) );
            return r;
        }
    //cleanup writelock
        librv0_rwlock_writelock_destroy_on_stack( &rl );
    //return null
        return 0;
    }

//create writelock with timeout
    librv0_object_writelock *librv0_object_try_create_writelock( librv0_object *t, unsigned long long ms )
    {
        librv0_object_writelock *r;
        librv0_rwlock_writelock rl;
    //attempt writelock acquire
        if( !librv0_rwlock_try_create_writelock_on_stack( &t->rwl, &rl, ms ) )
            return 0;
    //generate object writelock
        r = ( *( ( __librv0_object_gen_writelock_ptr )( t->func_gen_writelock ) ) )( t );
        if( r )
        {
            memcpy( &r->rwl, &rl, sizeof( librv0_rwlock_writelock ) );
            return r;
        }
    //cleanup writelock
        librv0_rwlock_writelock_destroy_on_stack( &rl );
    //return null
        return 0;
    }

//set generate readlock function pointer
    void __librv0_object_set_gen_readlock_func( librv0_object *t, __librv0_object_gen_readlock_ptr func )
    {
        t->func_gen_readlock = func;
    }

//set generate writelock function pointer
    void __librv0_object_set_gen_writelock_func( librv0_object *t, __librv0_object_gen_writelock_ptr func )
    {
        t->func_gen_writelock = func;
    }

//set deinit function pointer
    void __librv0_object_set_deinit_func( librv0_object *t, __librv0_object_deinit_ptr func )
    {
        t->func_deinit = func;
    }

//set deinit function pointer
    void __librv0_object_set_execute_func( librv0_object *t, __librv0_object_execute_ptr func )
    {
        t->func_execute = func;
    }

//default function to generate readlock
    librv0_object_readlock *__librv0_object_gen_readlock( librv0_object *t )
    {
        return librv0_object_readlock_create( t );
    }

//default function to generate writelock
    librv0_object_writelock *__librv0_object_gen_writelock( librv0_object *t )
    {
        return librv0_object_writelock_create( t );
    }

//default execute function
    void __librv0_object_execute( librv0_object *t, librv0_object_writelock *l, unsigned long long ticks, unsigned long long epoch )
    {
        return;
    }

//test object code
    bool librv0_object_test( void )
    {
        librv0_object *o;
        librv0_object_readlock *rl0, *rl1, *rl2;
        librv0_object_writelock *wl0, *wl1;
        librv0_object_ref *r0, *r1, *r2;
        bool b;

    //test creation and deletion
        o = librv0_object_create();
        librv0_object_destroy( &o );
        o = librv0_object_create();

    //create multiple readlocks
        rl0 = librv0_object_create_readlock( o );
        b = rl0 != 0;
        rl1 = librv0_object_try_create_readlock( o, 100 );
        b &= rl1 != 0;
        rl2 = librv0_object_try_create_readlock( o, 100 );
        b &= rl2 != 0;
        librv0_object_readlock_destroy( &rl0 );
        librv0_object_readlock_destroy( &rl1 );
        librv0_object_readlock_destroy( &rl2 );
        if( !b )
        {
            librv0_object_destroy( &o );
            return 0;
        }

    //create multiple writelocks
        wl0 = librv0_object_create_writelock( o );
        b = wl0 != 0;
        wl1 = librv0_object_try_create_writelock( o, 100 );
        b &= wl1 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_writelock_destroy( &wl1 );
        if( !b )
        {
            librv0_object_destroy( &o );
            return 0;
        }

    //create readlock during writelock
        wl0 = librv0_object_create_writelock( o );
        b = wl0 != 0;
        rl0 = librv0_object_try_create_readlock( o, 100 );
        b &= rl0 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_readlock_destroy( &rl0 );
        if( !b )
        {
            librv0_object_destroy( &o );
            return 0;
        }

    //create writelock during readlock
        rl0 = librv0_object_create_readlock( o );
        b = rl0 != 0;
        wl0 = librv0_object_try_create_writelock( o, 100 );
        b &= wl0 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_readlock_destroy( &rl0 );
        if( !b )
        {
            librv0_object_destroy( &o );
            return 0;
        }

    //create and destroy refs
        wl0 = librv0_object_create_writelock( o );
        if( !wl0 )
        {
            librv0_object_destroy( &o );
            return 0;
        }
        r0 = librv0_object_writelock_create_ref( wl0 );
        b = r0 != 0;
        r1 = librv0_object_writelock_create_ref( wl0 );
        b &= r1 != 0;
        r2 = librv0_object_writelock_create_ref( wl0 );
        b &= r2 != 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_ref_destroy( &r0 );
        librv0_object_ref_destroy( &r1 );
        librv0_object_ref_destroy( &r2 );
        if( !b )
        {
            librv0_object_destroy( &o );
            return 0;
        }

    //create ref for remaining tests
        wl0 = librv0_object_create_writelock( o );
        if( !wl0 )
        {
            librv0_object_destroy( &o );
            return 0;
        }
        r0 = librv0_object_writelock_create_ref( wl0 );
        librv0_object_writelock_destroy( &wl0 );
/*
    //create multiple readlocks
        rl0 = librv0_object_ref_create_readlock( r0 );
        b = rl0 != 0;
        rl1 = librv0_object_ref_try_create_readlock( r0, 100 );
        b &= rl1 != 0;
        rl2 = librv0_object_ref_try_create_readlock( r0, 100 );
        b &= rl2 != 0;
        librv0_object_readlock_destroy( &rl0 );
        librv0_object_readlock_destroy( &rl1 );
        librv0_object_readlock_destroy( &rl2 );
        if( !b )
        {
            librv0_object_ref_destroy( &r0 );
            librv0_object_destroy( &o );
            return 0;
        }

    //create readlock during writelock
        wl0 = librv0_object_ref_create_writelock( r0 );
        b = wl0 != 0;
        rl0 = librv0_object_ref_try_create_readlock( r0, 100 );
        b &= rl0 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_readlock_destroy( &rl0 );
        if( !b )
        {
            librv0_object_ref_destroy( &r0 );
            librv0_object_destroy( &o );
            return 0;
        }

    //create writelock during readlock
        rl0 = librv0_object_ref_create_readlock( r0 );
        b = rl0 != 0;
        wl0 = librv0_object_ref_try_create_writelock( r0, 100 );
        b &= wl0 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_readlock_destroy( &rl0 );
        if( !b )
        {
            librv0_object_ref_destroy( &r0 );
            librv0_object_destroy( &o );
            return 0;
        }
*/
    //destroy object and continue using ref
        librv0_object_destroy( &o );

    //try to use ref
        rl0 = librv0_object_ref_try_create_readlock( r0, 100 );
        b = rl0 == 0;
        wl0 = librv0_object_ref_try_create_writelock( r0, 100 );
        b &= wl0 == 0;
        librv0_object_writelock_destroy( &wl0 );
        librv0_object_readlock_destroy( &rl0 );
        if( !b )
        {
            librv0_object_ref_destroy( &r0 );
            return 0;
        }
        librv0_object_ref_destroy( &r0 );

        return 1;
    }

#endif // librv0_object_h

