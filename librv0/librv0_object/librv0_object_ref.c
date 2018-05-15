
#ifndef librv0_object_ref_c
#define librv0_object_ref_c

#include "librv0_object.h"
#include "librv0_object_readlock.h"
#include "librv0_object_writelock.h"
#include "librv0_object_ref.h"

//create ref
    librv0_object_ref *librv0_object_ref_create( librv0_object_writelock *l )
    {
        librv0_object_ref *r;
    //allocate memory
        r = malloc( sizeof( librv0_object_ref ) );
    //init struct
        if( r )
            __librv0_object_ref_init( r );
    //return
        return r;
    }

//destroy ref
    void librv0_object_ref_destroy( librv0_object_ref **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit
       ( *( ( __librv0_object_ref_deinit_ptr )( ( *t )->func_deinit ) ) )( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//init ref
    void __librv0_object_ref_init( librv0_object_ref *t )
    {
    //handle rwl ref init in writelock
    //set deinit pointer
        __librv0_object_ref_set_deinit_ptr( t, __librv0_object_ref_deinit );
    }

//deinit ref
    void __librv0_object_ref_deinit( librv0_object_ref *t )
    {
    //deinit rwl ref
        librv0_rwlock_ref_destroy_on_stack( &t->rwl );
    }

//set deinit func pointer
    void __librv0_object_ref_set_deinit_ptr( librv0_object_ref *t, __librv0_object_ref_deinit_ptr func )
    {
        t->func_deinit = func;
    }

//create readlock
    librv0_object_readlock *librv0_object_ref_create_readlock( librv0_object_ref *t )
    {
        librv0_object *prt;
        librv0_object_readlock *r;
    //lock parent
        prt = librv0_rwlock_ref_lock_ext_pointer( &t->rwl );
        if( !prt )
            return 0;
    //create readlock
        r = librv0_object_create_readlock( prt );
    //unlock parent
        librv0_rwlock_ref_unlock_ext_pointer( &t->rwl );
    //return
        return r;
    }

//create readlock with timeout
    librv0_object_readlock *librv0_object_ref_try_create_readlock( librv0_object_ref *t, unsigned long long ms )
    {
        librv0_object *prt;
        librv0_object_readlock *r;
    //lock parent
        prt = librv0_rwlock_ref_lock_ext_pointer( &t->rwl );
        if( !prt )
            return 0;
    //create readlock
        r = librv0_object_try_create_readlock( prt, ms );
    //unlock parent
        librv0_rwlock_ref_unlock_ext_pointer( &t->rwl );
    //return
        return r;
    }

//create writelock
    librv0_object_writelock *librv0_object_ref_create_writelock( librv0_object_ref *t )
    {
        librv0_object *prt;
        librv0_object_writelock *r;
    //lock parent
        prt = librv0_rwlock_ref_lock_ext_pointer( &t->rwl );
        if( !prt )
            return 0;
    //create readlock
        r = librv0_object_create_writelock( prt );
    //unlock parent
        librv0_rwlock_ref_unlock_ext_pointer( &t->rwl );
    //return
        return r;
    }

//create writelock with timeout
    librv0_object_writelock *librv0_object_ref_try_create_writelock( librv0_object_ref *t, unsigned long long ms )
    {
        librv0_object *prt;
        librv0_object_writelock *r;
    //lock parent
        prt = librv0_rwlock_ref_lock_ext_pointer( &t->rwl );
        if( !prt )
            return 0;
    //create readlock
        r = librv0_object_try_create_writelock( prt, ms );
    //unlock parent
        librv0_rwlock_ref_unlock_ext_pointer( &t->rwl );
    //return
        return r;
    }

#endif // librv0_object_ref_h

