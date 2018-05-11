
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



#endif // librv0_object_readlock_h
