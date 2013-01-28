
#ifndef _DOREMIR_THREAD_FUTURE
#define _DOREMIR_THREAD_FUTURE

#include <doremir/std.h>
#include <doremir/thread.h>

/** @defgroup Doremir Doremir
    @{
    @defgroup DoremirThread Thread
    @{
    @defgroup DoremirThreadFuture Future
    @{
    */

typedef struct _doremir_thread_future_t * doremir_thread_future_t;
typedef doremir_ptr_t doremir_thread_future_value_t;
doremir_thread_future_t doremir_thread_future_create(doremir_nullary_t,
                                                     doremir_ptr_t);
void doremir_thread_future_pause(doremir_thread_future_t);
void doremir_thread_future_resume(doremir_thread_future_t);
bool doremir_thread_future_is_done(doremir_thread_future_t);
doremir_thread_future_value_t doremir_thread_future_get(doremir_thread_future_t);
void doremir_thread_future_cancel(doremir_thread_future_t);

/** @}
    @}
    @}
    */

#endif // _DOREMIR_THREAD_FUTURE

