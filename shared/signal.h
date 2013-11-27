
#include <fa/priority_queue.h>

// TODO formalize better
struct _state_base_t {
    double *inputs;
    void   *_;
    int     count;
    double  rate;
    // ...
};

#define kVectorMode false

#define kMaxVectorSize 128
#define kDefVectorSize 128

#define kDefSampleRate 44100
#define kAudioSchedulerInterval 5

typedef struct _state_t *state_t;
typedef struct _state_base_t *state_base_t;

list_t fa_signal_get_procs(fa_signal_t signal2);
state_t new_state();
void add_custom_proc(fa_signal_custom_processor_t* proc, state_t state);
void delete_state(state_t state);
void inc_state1(state_t state);           
ptr_t run_simple_action(state_t state, fa_action_t action);
void run_actions(priority_queue_t controls, fa_time_t now, unary_t function, ptr_t data);
void run_custom_procs(int when, state_t state);
double step(signal_t signal, state_t state);
void step_vector(signal_t signal, state_t state, int count, double* out);
fa_signal_t fa_signal_simplify(fa_signal_t signal2);