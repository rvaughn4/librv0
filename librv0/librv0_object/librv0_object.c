
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
        __librv0_object_set_gen_ref_func( t, __librv0_object_gen_ref );
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

//set generate ref function pointer
    void __librv0_object_set_gen_ref_func( librv0_object *t, __librv0_object_gen_ref_ptr func )
    {
        t->func_gen_ref = func;
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

//default function to generate ref
    librv0_object_ref *__librv0_object_gen_ref( librv0_object *t, librv0_object_writelock *l )
    {
        return 0;
    }

//default execute function
    void __librv0_object_execute( librv0_object *t, librv0_object_writelock *l, unsigned long long ticks, unsigned long long epoch )
    {
        return;
    }

#endif // librv0_object_h

