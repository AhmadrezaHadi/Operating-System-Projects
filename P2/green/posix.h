#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include<pthread.h>
#define green_t pthread_t
#define green_create(X,Y,Z) pthread_create(X,NULL,Y,Z)
#define green_yield() pthread_yield()
int green_join(green_t * thread, void ** val)
{
    return pthread_join(*thread,val);
}
