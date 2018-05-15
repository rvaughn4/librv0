
#ifndef librv0_object_writelock_h
#define librv0_object_writelock_h

#include "../librv0_rwlock/librv0_rwlock_writelock.h"
#include "librv0_object.h"

//object writelock struct
    typedef struct
    {
    //writelock
        librv0_rwlock_writelock  rwl;
    //parent object
        librv0_object           *prt;
    //deinit func pointer
        void                    *func_deinit;
    //gen ref func pointer
        void                    *func_gen_ref;
    } librv0_object_writelock;

#include "librv0_object_ref.h"

//function to deinit
    typedef void (*__librv0_object_writelock_deinit_ptr)( librv0_object_writelock *t );
//function to generate ref
    typedef librv0_object_ref *(*__librv0_object_writelock_gen_ref_ptr)( librv0_object_writelock *l );
//create object writelock struct
    librv0_object_writelock *librv0_object_writelock_create( librv0_object *prt );
//destroy object writelock struct
    void librv0_object_writelock_destroy( librv0_object_writelock **t );
//init object writelock struct
    void __librv0_object_writelock_init( librv0_object_writelock *t, librv0_object *prt );
//deinit object writelock struct
    void __librv0_object_writelock_deinit( librv0_object_writelock *t );
//generate ref
    librv0_object_ref *__librv0_object_writelock_gen_ref( librv0_object_writelock *t );
//set deinit function
    void __librv0_object_writelock_set_deinit_func( librv0_object_writelock *t, __librv0_object_writelock_deinit_ptr func );
//set generate ref function pointer
    void __librv0_object_writelock_set_gen_ref_func( librv0_object_writelock *t, __librv0_object_writelock_gen_ref_ptr func );
//return id
    unsigned long long librv0_object_writelock_get_id( librv0_object_writelock *t );
//return last ticks ran
    unsigned long long librv0_object_writelock_get_last_ticks( librv0_object_writelock *t );
//return ticks until next run
    unsigned long long librv0_object_writelock_get_ticks_until_run( librv0_object_writelock *t, unsigned long long ticks_ms );
//return last epoch ran
    unsigned long long librv0_object_writelock_get_last_time( librv0_object_writelock *t );
//return priority
    int librv0_object_writelock_get_priority( librv0_object_writelock *t );
//return delay between runs in ms ticks
    unsigned long long librv0_object_writelock_get_delay( librv0_object_writelock *t );
//set id
    void librv0_object_writelock_set_id( librv0_object_writelock *t, unsigned long long id );
//set priority
    void librv0_object_writelock_set_priority( librv0_object_writelock *t, int p_bigger_is_higher );
//execute
    void librv0_object_writelock_execute( librv0_object_writelock *t, unsigned long long ms_ticks, unsigned long long epoch_ms );
//create ref
    librv0_object_ref *librv0_object_writelock_create_ref( librv0_object_writelock *t );

#endif // librv0_object_writelock_h
