
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
    } librv0_object_ref;

#endif // librv0_object_ref_h
