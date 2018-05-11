
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
    } librv0_object_writelock;



#endif // librv0_object_readlock_h

