
#ifndef librv0_object_h
#define librv0_object_h

#include "../librv0_rwlock/librv0_rwlock.h"

//object struct
    typedef struct
    {
        librv0_rwlock       rwl;
        unsigned long long  id;
        unsigned long long  lastTicks, delayTicks;
        unsigned long long  lastEpoch;
        int                 priority;
        bool                bHasStart, bDoRun;
        void                *func_gen_readlock;
        void                *func_gen_writelock;
        void                *func_gen_ref;
        void                *func_deinit;
        void                *func_execute;
        void                *func_execute_start;
        void                *func_execute_stop;
    } librv0_object;

#include "librv0_object_readlock.h"
#include "librv0_object_writelock.h"
#include "librv0_object_ref.h"

//create new object
    librv0_object *librv0_object_create( void );
//create new object on stack
    librv0_object *librv0_object_create_on_stack( librv0_object *t );
//destroy object
    void librv0_object_destroy( librv0_object **t );
//destroy object on stack
    void librv0_object_destroy_on_stack( librv0_object *t );
//init struct
    void __librv0_object_init( librv0_object *t );
//deinit struct
    void __librv0_object_deinit( librv0_object *t );
//generate readlock
    librv0_object_readlock *librv0_object_gen_readlock_override( librv0_object *t );
//generate writelock
    librv0_object_writelock *librv0_object_gen_writelock_override( librv0_object *t );
//generate ref
    librv0_object_ref *librv0_object_gen_ref_override( librv0_object *t, librv0_object_writelock *tl );
//create readlock
    librv0_object_readlock *librv0_object_create_readlock( librv0_object *t );
//create readlock with timeout
    librv0_object_readlock *librv0_object_try_create_readlock( librv0_object *t, unsigned long long ms );
//create readlock on stack
    librv0_object_readlock *librv0_object_create_readlock_on_stack( librv0_object *t, librv0_object_readlock *l );
//create readlock on stack with timeout
    librv0_object_readlock *librv0_object_try_create_readlock_on_stack( librv0_object *t, librv0_object_readlock *l, unsigned long long ms );
//create writelock
    librv0_object_writelock *librv0_object_create_writelock( librv0_object *t );
//create writelock with timeout
    librv0_object_writelock *librv0_object_try_create_writelock( librv0_object *t, unsigned long long ms );
//create writelock on stack
    librv0_object_writelock *librv0_object_create_writelock_on_stack( librv0_object *t, librv0_object_writelock *l );
//create writelock on stack with timeout
    librv0_object_writelock *librv0_object_try_create_writelock_on_stack( librv0_object *t, librv0_object_writelock *l, unsigned long long ms );

#endif // librv0_object_h
