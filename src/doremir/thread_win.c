
#include <doremir/thread.h>

struct _doremir_thread_t
{
};

struct _doremir_thread_mutex_t
{
};

struct _doremir_thread_condition_t
{
};

/** Creates a new thread.
 */
doremir_thread_t doremir_thread_create_thread()
{
}

void doremir_thread_join(doremir_thread_t t)
{
}

void doremir_thread_detach(doremir_thread_t t)
{
}

doremir_thread_mutex_t doremir_thread_create_mutex()
{
}

void doremir_thread_destroy_mutex(doremir_thread_mutex_t m)
{                     
}

bool doremir_thread_lock(doremir_thread_mutex_t t)
{ 
    int r = 0;
    if (r != 0)
        ; // TODO fatal
}

bool doremir_thread_try_lock(doremir_thread_mutex_t t)
{
    int r = 0;
    if (r != 0)
        ; // TODO fatal
}

bool doremir_thread_unlock(doremir_thread_mutex_t t)
{
    int r = 0;
    if (r != 0)
        ; // TODO fatal
}

doremir_thread_condition_t doremir_thread_create_condition()
{
    int r = 0;
    if (r != 0)
        ; // TODO fatal
    // TODO return c
}

void doremir_thread_wait_for(doremir_thread_condition_t c)
{
    int r = 0;
    // TODO free
    if (r != 0)
        ; // TODO fatal
}

void doremir_thread_notify(doremir_thread_condition_t c)
{
    int r = 0;
    if (r != 0)
        ; // TODO fatal
}

void doremir_thread_notify_all(doremir_thread_condition_t c)
{
    int r = 0;
    if (r != 0)
        ; // TODO fatal
}

