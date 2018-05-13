
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
    } librv0_object;

#include "librv0_object_readlock.h"
#include "librv0_object_writelock.h"
#include "librv0_object_ref.h"

//function to generate readlock
    typedef librv0_object_readlock *(*__librv0_object_gen_readlock_ptr)( librv0_object *t );
//function to generate writelock
    typedef librv0_object_writelock *(*__librv0_object_gen_writelock_ptr)( librv0_object *t );
//function to generate ref
    typedef librv0_object_ref *(*__librv0_object_gen_ref_ptr)( librv0_object *t, librv0_object_writelock *l );
//function to deinit
    typedef void (*__librv0_object_deinit_ptr)( librv0_object *t );
//function to execute
    typedef void (*__librv0_object_execute_ptr)( librv0_object *t, librv0_object_writelock *l, unsigned long long ticks, unsigned long long epoch );
//create new object
    librv0_object *librv0_object_create( void );
//destroy object
    void librv0_object_destroy( librv0_object **t );
//init struct
    void __librv0_object_init( librv0_object *t );
//deinit struct
    void __librv0_object_deinit( librv0_object *t );
//create readlock
    librv0_object_readlock *librv0_object_create_readlock( librv0_object *t );
//create readlock with timeout
    librv0_object_readlock *librv0_object_try_create_readlock( librv0_object *t, unsigned long long ms );
//create writelock
    librv0_object_writelock *librv0_object_create_writelock( librv0_object *t );
//create writelock with timeout
    librv0_object_writelock *librv0_object_try_create_writelock( librv0_object *t, unsigned long long ms );
//set generate readlock function pointer
    void __librv0_object_set_gen_readlock_func( librv0_object *t, __librv0_object_gen_readlock_ptr );
//set generate writelock function pointer
    void __librv0_object_set_gen_writelock_func( librv0_object *t, __librv0_object_gen_writelock_ptr );
//set generate ref function pointer
    void __librv0_object_set_gen_ref_func( librv0_object *t, __librv0_object_gen_ref_ptr );
//set deinit function pointer
    void __librv0_object_set_deinit_func( librv0_object *t, __librv0_object_deinit_ptr );
//set deinit function pointer
    void __librv0_object_set_execute_func( librv0_object *t, __librv0_object_execute_ptr );
//default function to generate readlock
    librv0_object_readlock *__librv0_object_gen_readlock( librv0_object *t );
//default function to generate writelock
    librv0_object_writelock *__librv0_object_gen_writelock( librv0_object *t );
//default function to generate ref
    librv0_object_ref *__librv0_object_gen_ref( librv0_object *t, librv0_object_writelock *l );
//default execute function
    void __librv0_object_execute( librv0_object *t, librv0_object_writelock *l, unsigned long long ticks, unsigned long long epoch );

#endif // librv0_object_h
