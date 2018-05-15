#include <stdio.h>
#include <stdlib.h>

//#include "librv0/librv0_rwlock/librv0_rwlock.h"
#include "librv0/librv0_object/librv0_object.h"

int main()
{
    printf("Hello world!\n");

    librv0_object_test();
    //librv0_rwlock_test();
    return 0;
}
