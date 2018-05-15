
#ifndef librv0_object_writelock_c
#define librv0_object_writelock_c

#include "librv0_object.h"
#include "librv0_object_writelock.h"

//create object writelock struct
    librv0_object_writelock *librv0_object_writelock_create( librv0_object *prt )
    {
        librv0_object_writelock *r;
    //allocate memory
        r = malloc( sizeof( librv0_object_writelock ) );
    //init struct
        if( r )
            __librv0_object_writelock_init( r, prt );
    //return
        return r;
    }

//destroy object writelock struct
    void librv0_object_writelock_destroy( librv0_object_writelock **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        ( *( (__librv0_object_writelock_deinit_ptr )( ( *t )->func_deinit ) ) )( *t );
    //release memory
        free( *t );
    //set to null
        *t = 0;
    }

//init object writelock struct
    void __librv0_object_writelock_init( librv0_object_writelock *t, librv0_object *prt )
    {
    //rwlock init handled in object
        t->prt = prt;
    //set func pointers
        __librv0_object_writelock_set_deinit_func( t, __librv0_object_writelock_deinit );
        __librv0_object_writelock_set_gen_ref_func( t, __librv0_object_writelock_gen_ref );
    }

//deinit object writelock struct
    void __librv0_object_writelock_deinit( librv0_object_writelock *t )
    {
    //handle rwl
        librv0_rwlock_writelock_destroy_on_stack( &t->rwl );
    }

//generate ref
    librv0_object_ref *__librv0_object_writelock_gen_ref( librv0_object_writelock *t )
    {
        return librv0_object_ref_create( t );
    }

//set deinit function
    void __librv0_object_writelock_set_deinit_func( librv0_object_writelock *t, __librv0_object_writelock_deinit_ptr func )
    {
        t->func_deinit = func;
    }

//set generate ref function pointer
    void __librv0_object_writelock_set_gen_ref_func( librv0_object_writelock *t, __librv0_object_writelock_gen_ref_ptr func )
    {
        t->func_gen_ref = func;
    }

//return id
    unsigned long long librv0_object_writelock_get_id( librv0_object_writelock *t )
    {
        return t->prt->id;
    }

//return last ticks ran
    unsigned long long librv0_object_writelock_get_last_ticks( librv0_object_writelock *t )
    {
        return t->prt->lastTicks;
    }

//return ticks until next run
    unsigned long long librv0_object_writelock_get_ticks_until_run( librv0_object_writelock *t, unsigned long long ticks_ms )
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
    unsigned long long librv0_object_writelock_get_last_time( librv0_object_writelock *t )
    {
        return t->prt->lastEpoch;
    }

//return priority
    int librv0_object_writelock_get_priority( librv0_object_writelock *t )
    {
        return t->prt->priority;
    }

//return delay between runs in ms ticks
    unsigned long long librv0_object_writelock_get_delay( librv0_object_writelock *t )
    {
        return t->prt->delayTicks;
    }

//set id
    void librv0_object_writelock_set_id( librv0_object_writelock *t, unsigned long long id )
    {
        t->prt->id = id;
    }

//set priority
    void librv0_object_writelock_set_priority( librv0_object_writelock *t, int p_bigger_is_higher )
    {
        t->prt->priority = p_bigger_is_higher;
    }

//execute
    void librv0_object_writelock_execute( librv0_object_writelock *t, unsigned long long ms_ticks, unsigned long long epoch_ms )
    {
        ( *( ( __librv0_object_execute_ptr )( t->prt->func_execute ) ) )( t->prt, t, ms_ticks, epoch_ms );
    }

//create ref
    librv0_object_ref *librv0_object_writelock_create_ref( librv0_object_writelock *t )
    {
        librv0_object_ref *r;
    //create ref
        r = ( *( ( __librv0_object_writelock_gen_ref_ptr )( t->func_gen_ref ) ) )( t );
        if( !r )
            return 0;
    //create ref rwlock
        if( librv0_rwlock_ref_create_on_stack( t->rwl.prt, &t->rwl, &r->rwl, t->prt ) )
            return r;
    //failure, cleanup
        librv0_object_ref_destroy( &r );
        return 0;
    }

#endif // librv0_object_writelock_h

