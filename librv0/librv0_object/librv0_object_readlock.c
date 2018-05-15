
#ifndef librv0_object_readlock_c
#define librv0_object_readlock_c

#include "librv0_object.h"
#include "librv0_object_readlock.h"

//create object readlock struct
    librv0_object_readlock *librv0_object_readlock_create( librv0_object *prt )
    {
        librv0_object_readlock *r;
    //allocate memory
        r = malloc( sizeof( librv0_object_readlock ) );
    //init struct
        if( r )
            __librv0_object_readlock_init( r, prt );
    //return
        return r;
    }

//destroy object readlock struct
    void librv0_object_readlock_destroy( librv0_object_readlock **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        ( *( ( __librv0_object_readlock_deinit_ptr )( ( *t )->func_deinit ) ) )( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//init object readlock struct
    void __librv0_object_readlock_init( librv0_object_readlock *t, librv0_object *prt )
    {
    //handle rwlock inside of object->create_readlock and not here
        t->prt = prt;
    //set func pointers
        __librv0_object_readlock_set_deinit_func( t, __librv0_object_readlock_deinit );
    }

//deinit object readlock struct
    void __librv0_object_readlock_deinit( librv0_object_readlock *t )
    {
    //handle rwlock
        librv0_rwlock_readlock_destroy_on_stack( &t->rwl );
    }

//set deinit function
    void __librv0_object_readlock_set_deinit_func( librv0_object_readlock *t, __librv0_object_readlock_deinit_ptr func )
    {
        t->func_deinit = func;
    }

//return id
    unsigned long long librv0_object_readlock_get_id( librv0_object_readlock *t )
    {
        return t->prt->id;
    }

//return last ticks ran
    unsigned long long librv0_object_readlock_get_last_ticks( librv0_object_readlock *t )
    {
        return t->prt->lastTicks;
    }

//return ticks until next run
    unsigned long long librv0_object_readlock_get_ticks_until_run( librv0_object_readlock *t, unsigned long long ticks_ms )
    {
        unsigned long long dt;
    //compute next run time
        dt = t->prt->lastTicks + t->prt->delayTicks;
    //check for integer overflow
        if( dt < ticks_ms )
            return 0;
    //return difference
        return dt - ticks_ms;
    }

//return last epoch ran
    unsigned long long librv0_object_readlock_get_last_time( librv0_object_readlock *t )
    {
        return t->prt->lastEpoch;
    }

//return priority
    int librv0_object_readlock_get_priority( librv0_object_readlock *t )
    {
        return t->prt->priority;
    }

//return delay between runs in ms ticks
    unsigned long long librv0_object_readlock_get_delay( librv0_object_readlock *t )
    {
        return t->prt->delayTicks;
    }

#endif // librv0_object_readlock_h

