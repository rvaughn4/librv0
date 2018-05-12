
#ifndef librv0_object_readlock_h
#define librv0_object_readlock_h

#include "../librv0_rwlock/librv0_rwlock_readlock.h"
#include "librv0_object.h"

//object readlock struct
    typedef struct
    {
    //readlock
        librv0_rwlock_readlock  rwl;
    //parent object
        librv0_object           *prt;
    } librv0_object_readlock;

//create object readlock struct
    librv0_object_readlock *librv0_object_readlock_create( librv0_object *prt );
//create object readlock struct on stack
    librv0_object_readlock *librv0_object_readlock_create_on_stack( librv0_object_readlock *t, librv0_object *prt );
//destroy object readlock struct
    void librv0_object_readlock_destroy( librv0_object_readlock **t );
//destroy object readlock struct on stack
    void librv0_object_readlock_destroy_on_stack( librv0_object_readlock *t );
//init object readlock struct
    void __librv0_object_readlock_init( librv0_object_readlock *t, librv0_object *prt );
//deinit object readlock struct
    void __librv0_object_readlock_deinit( librv0_object_readlock *t );
//return id
    unsigned long long librv0_object_readlock_get_id( librv0_object_readlock *t );
//return last ticks ran
    unsigned long long librv0_object_readlock_get_last_ticks( librv0_object_readlock *t );
//return ticks until next run
    unsigned long long librv0_object_readlock_get_ticks_until_run( librv0_object_readlock *t, unsigned long long ticks_ms );
//return last epoch ran
    unsigned long long librv0_object_readlock_get_last_time( librv0_object_readlock *t );
//return priority
    int librv0_object_readlock_get_priority( librv0_object_readlock *t );
//return delay between runs in ms ticks
    unsigned long long librv0_object_readlock_get_delay( librv0_object_readlock *t );

#endif // librv0_object_readlock_h
