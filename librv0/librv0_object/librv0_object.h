
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
    } librv0_object;

#endif // librv0_object_h
