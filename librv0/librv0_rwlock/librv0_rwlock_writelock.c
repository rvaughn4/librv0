
#ifndef librv0_rwlock_writelock_c
#define librv0_rwlock_writelock_c

#include "librv0_rwlock_writelock.h"
#include <stdlib.h>

//create new writelock
    librv0_rwlock_writelock *librv0_rwlock_writelock_create( librv0_rwlock *prt )
    {
        librv0_rwlock_writelock *r;
    //allocate memory
        r = malloc( sizeof( librv0_rwlock_writelock ) );
    //init struct
        if( librv0_rwlock_writelock_create_on_stack( r, prt ) )
            return r;
    //release memory on error and return
        free( r );
        return 0;
    }

//create new readlock
    librv0_rwlock_writelock *librv0_rwlock_writelock_create_on_stack( librv0_rwlock_writelock *t, librv0_rwlock *prt )
    {
    //test pointers
        if( !t || !prt )
            return 0;
    //init struct
        __librv0_rwlock_writelock_init( t, prt );
    //return
        return t;
    }

//destroy writelock
    void librv0_rwlock_writelock_destroy( librv0_rwlock_writelock **t )
    {
    //test pointers
        if( !t || !*t )
            return;
    //deinit struct
        librv0_rwlock_writelock_destroy_on_stack( *t );
    //release memory
        free( *t );
    //set pointer to null
        *t = 0;
    }

//destroy readlock
    void librv0_rwlock_writelock_destroy_on_stack( librv0_rwlock_writelock *t )
    {
    //test pointers
        if( !t )
            return;
    //deinit struct
        __librv0_rwlock_writelock_deinit( t );
    }

//init writelock
    void __librv0_rwlock_writelock_init( librv0_rwlock_writelock *t, librv0_rwlock *prt )
    {
    //set pointers
        t->prt = prt;
        t->rwl = &prt->rwl;
    }

//deinit writelock
    void __librv0_rwlock_writelock_deinit( librv0_rwlock_writelock *t )
    {
    //unlock parent struct
        pthread_rwlock_unlock( t->rwl );
    }

//create new reference
    librv0_rwlock_ref *librv0_rwlock_writelock_create_ref( librv0_rwlock_writelock *t, void *ext_ptr )
    {
        librv0_rwlock_ref *r;
    //allocate memory
        r = malloc( sizeof( librv0_rwlock_ref ) );
        if( !r )
            return 0;
    //create ref
        if( librv0_rwlock_writelock_create_ref_on_stack( t, r, ext_ptr ) )
            return r;
    //failure, release memory
        free( r );
        return 0;
    }

//create new reference on stack
    librv0_rwlock_ref *librv0_rwlock_writelock_create_ref_on_stack( librv0_rwlock_writelock *t, librv0_rwlock_ref *r, void *ext_ptr )
    {
        if( !librv0_rwlock_ref_create_on_stack( t->prt, t, r, ext_ptr ) )
            return 0;
        if( !__librv0_rwlock_insert_reflist( t->prt, r ) )
        {
            if( !__librv0_rwlock_resize_reflist( t->prt ) )
            {
                librv0_rwlock_ref_destroy_on_stack( r );
                return 0;
            }
            else
            {
                if( !__librv0_rwlock_insert_reflist( t->prt, r ) )
                {
                    librv0_rwlock_ref_destroy_on_stack( r );
                    return 0;
                }
            }
        }
        return r;
    }

#endif // librv0_rwlock_writelock_h


