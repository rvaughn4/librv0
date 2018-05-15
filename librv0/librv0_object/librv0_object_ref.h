
#ifndef librv0_object_ref_h
#define librv0_object_ref_h

#include "librv0_object.h"
#include "librv0_object_readlock.h"
#include "librv0_object_writelock.h"
#include "../librv0_rwlock/librv0_rwlock_ref.h"

//object ref
    typedef struct
    {
        librv0_rwlock_ref rwl;
    //deinit func pointer
        void                    *func_deinit;
    } librv0_object_ref;

//function to deinit
    typedef void (*__librv0_object_ref_deinit_ptr)( librv0_object_ref *t );
//create ref
    librv0_object_ref *librv0_object_ref_create( librv0_object_writelock *l );
//destroy ref
    void librv0_object_ref_destroy( librv0_object_ref **t );
//init ref
    void __librv0_object_ref_init( librv0_object_ref *t );
//deinit ref
    void __librv0_object_ref_deinit( librv0_object_ref *t );
//set deinit func pointer
    void __librv0_object_ref_set_deinit_ptr( librv0_object_ref *t, __librv0_object_ref_deinit_ptr func );
//create readlock
    librv0_object_readlock *librv0_object_ref_create_readlock( librv0_object_ref *t );
//create readlock with timeout
    librv0_object_readlock *librv0_object_ref_try_create_readlock( librv0_object_ref *t, unsigned long long ms );
//create writelock
    librv0_object_writelock *librv0_object_ref_create_writelock( librv0_object_ref *t );
//create writelock with timeout
    librv0_object_writelock *librv0_object_ref_try_create_writelock( librv0_object_ref *t, unsigned long long ms );

#endif // librv0_object_ref_h
