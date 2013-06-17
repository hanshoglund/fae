
/*
    DoReMIR Audio Engine
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.
 */

#include <doremir/processor.h>
#include <doremir/processor/unary.h>
#include <doremir/processor/binary.h>
#include <doremir/processor/split.h>
#include <doremir/processor/seq.h>
#include <doremir/processor/par.h>
#include <doremir/processor/loop.h>
#include <doremir/processor/delay.h>
#include <doremir/system/directory.h>
#include <doremir/util.h>


/** Return the input type of the given processor.

    @param proc             A processor.
 */
doremir_type_t doremir_processor_input_type(doremir_processor_t proc)
{
    assert(doremir_interface(doremir_processor_interface_i, proc) && "Must implement Processor");
    return ((proc_interface_t *) doremir_interface(doremir_processor_interface_i, proc))->input_type(proc);
}

/** Return the output type of the given processor.

    @param proc             A processor.
 */
doremir_type_t doremir_processor_output_type(doremir_processor_t proc)
{
    assert(doremir_interface(doremir_processor_interface_i, proc) && "Must implement Processor");
    return ((proc_interface_t *) doremir_interface(doremir_processor_interface_i, proc))->output_type(proc);
}

/** Return the output type of the given processor.
    @param proc             A processor.
 */
size_t doremir_processor_buffer_size(doremir_type_frames_t frames, doremir_processor_t proc)
{
    assert(doremir_interface(doremir_processor_interface_i, proc) && "Must implement Processor");
    return ((proc_interface_t *) doremir_interface(doremir_processor_interface_i, proc))->buffer_size(frames, proc);
}

doremir_graph_t doremir_processor_graph(doremir_processor_t proc,
                                        doremir_processor_info_t *info,
                                        doremir_graph_t graph)
{
    assert(doremir_interface(doremir_processor_interface_i, proc) && "Must implement Processor");
    return ((proc_interface_t *) doremir_interface(doremir_processor_interface_i, proc))->graph(proc, info, graph);
}

/** Returns the address of the given processor.
    @param proc             A processor.
 */
doremir_ptr_t doremir_processor_address(doremir_processor_t proc)
{
    return proc;
}

/** Write a graph representation of the given processor
    (in the dot language) to the given file.
 */
void doremir_processor_write_graph(doremir_processor_t proc,
                                   doremir_string_file_path_t path)
{
    graph_t graph = doremir_graph_empty();
    doremir_processor_info_t info = {
        .buf_offset = 0,
        .buf_step   = 1,
        .buf_loop   = 1,
        .buf_seq    = 1
    };
    graph = doremir_processor_graph(proc, &info, graph);
    doremir_system_directory_write_file(path, doremir_graph_to_dot(string(""), string(""), graph));
}


inline static void *identity(void *x, void *a)
{
    return a;
}
inline static void *constant(void *x, void *a)
{
    return x;
}

/** Create an identity processor.

    This processor returns input of the given type unmodified.

    @param type             Type of input.
    @return                 A processor.
 */
doremir_processor_t doremir_processor_identity(doremir_type_t type)
{
    return (processor_t) doremir_processor_unary_create(type, type, identity, NULL);
}

/** Create a constant processor.

    This processor consumes input of the given type, and returns a constant value of
    the given type.

    @param input_type       Type of input.
    @param output_type      Type of output.
    @param value            Pointer to a buffer containing the value.
 */
doremir_processor_t doremir_processor_constant(doremir_type_t   input_type,
                                               doremir_type_t   output_type,
                                               doremir_ptr_t    value)
{
    // TODO defensively copy, or trust the user?
    // TODO if we are closing over a frame, should we multiply here instead of trusting the user to?
    return (processor_t) doremir_processor_unary_create(input_type, output_type, constant, NULL);
}

/** Create a delay processor.

    This processor delays its input by the given value.

    @param input_type       Type of input.
    @param samples          Number of samples.
 */
doremir_processor_t doremir_processor_delay(doremir_type_t  type,
                                            size_t          samples)
{
    assert(false && "Not implemented");
}

/** Create a split processor.
    @param input_type       Type of input.
 */
doremir_processor_t doremir_processor_split(doremir_type_t type)
{
    return (processor_t) doremir_processor_split_create(type);
}

/** Lift a unary function to a processor.

    The given types must be simple types or frames types.

    The given function will receive a pointer to a single value of the given input
    types and is expected to return a pointer to a value of the proper output type.
    The received pointer may be used as return value.

    @param input_type       Input type. Must be unit, a simple type or a frame type containing a simple type.
    @param output_type      Type of output. Must be unit, a simple type or a frame type containing a simple type.
    @param function         Function to be lifted.
    @param data             Value to be passed to function.
    @return                 A processor.
 */
doremir_processor_t doremir_processor_unary
(
    doremir_type_t  input_type,
    doremir_type_t  output_type,
    doremir_unary_t function,
    doremir_ptr_t   data
)
{
    return (processor_t)
           doremir_processor_unary_create(
               input_type, output_type,
               function, data
           );
}

/** Lift a binary function to a processor.

    The given types must be simple types or frames types.

    The given function will receive pointers to single values of the given input
    types and is expected to return a pointer to a value of the proper output type.
    The first received pointer may be used as return value.

    @param input_type1      Type of first input.
    @param input_type2      Type of second input.
    @param output_type      Type of output.
    @param function         Function to be lifted.
    @param data             Value to be passed to function.
    @return                 A processor.
 */
doremir_processor_t doremir_processor_binary
(
    doremir_type_t   input_type1,
    doremir_type_t   input_type2,
    doremir_type_t   output_type,
    doremir_binary_t function,
    doremir_ptr_t    data
)
{
    return (processor_t)
           doremir_processor_binary_create(
               input_type1, input_type2, output_type,
               function, data
           );
}


/** Create a processor by composing the given processors in parallel.

    The input and output types are pairs of the input and output types of the given processors.

        \f[
            \frac
                {p \in A \rightarrow B \qquad q \in C \rightarrow D}
                {par(p,q) \in (A,C) \rightarrow (B,D)}
        \f]

    @param proc             Left processor.
    @param proc             Right processor.
    @return                 A new processor, or an error.
 */
doremir_processor_t doremir_processor_parallel(doremir_processor_t proc1,
                                               doremir_processor_t proc2)
{
    return (processor_t)
           doremir_processor_par_create(proc1, proc2);
}

/** Create a processor by composing the given processors in sequence.

    The output type of the first processor must match the input type of the
    second processor, or an error is returned.

        \f[
            \frac
                {p \in A \rightarrow B \qquad q \in B \rightarrow C}
                {seq(p,q) \in A \rightarrow C}
        \f]

    @param proc             First processor.
    @param proc             Second processor.
    @return                 A new processor, or an error.
 */
doremir_processor_t doremir_processor_sequence(doremir_processor_t proc1,
                                               doremir_processor_t proc2)
{
    return (processor_t) doremir_processor_seq_create(proc1, proc2);
}

/** Create a processor by composing the given processors in reverse order.
    This function is analogous to function composition.

    The output type of the second processor must match the input type of the
    first processor, or an error is returned.

        \f[
            \frac
                {p \in B \rightarrow C \qquad q \in A \rightarrow B}
                {compose(p,q) \in A \rightarrow C}
        \f]

    @param proc             First processor.
    @param proc             Second processor.
    @return                 A new processor, or an error.
 */
doremir_processor_t doremir_processor_compose(doremir_processor_t proc1, doremir_processor_t proc2)
{
    return (processor_t) doremir_processor_seq_create(proc2, proc1);
}

/** Create a processor by feeding the given processor back into itself.

        \f[
            \frac
                {p \in (C,A) \rightarrow (C,B) }
                { loop(p) \in A \rightarrow B }
        \f]

    @param proc             Processor to close feedback loop over.
    @return                 A new processor, or an error.
 */
doremir_processor_t doremir_processor_loop(doremir_processor_t proc)
{
    return (processor_t) doremir_processor_loop_create(proc);
}







uint8_t prim_add_i8_i8(ptr_t c, uint8_t a, uint8_t b)
{
    return a + b;
};
float   prim_add_f32_f32(ptr_t c, float a, float b)
{
    return a + b;
};

doremir_processor_t doremir_processor_add(doremir_type_t type)
{
    return (processor_t) doremir_processor_unary(type(i8), type(i8), (unary_t) prim_add_i8_i8, NULL);
}

doremir_processor_t doremir_processor_subtract(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_multiply(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_divide(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_modulo(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_absolute(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_not(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_and(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_or(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_xor(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_bit_not(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_bit_and(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_bit_or(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_bit_xor(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_shift_left(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_shift_right(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_equal(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_less_than(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_greater_than(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_less_than_equal(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_greater_than_equal(doremir_type_t type)
{
    assert(false && "Not implemented");
}


inline static void *apply_1f21f(void *f, void *a)
{
    typedef float(*f2f)(float);
    f2f     g = (f2f) f;
    float  *b = a;
    b[0] = g(b[0]);
    return a;
}

doremir_processor_t doremir_processor_acos(doremir_type_t type)
{
    // assert float, double or long double
    return doremir_processor_unary(type(f32), type(f32), apply_1f21f, acos);
}

doremir_processor_t doremir_processor_asin(doremir_type_t type)
{
    return doremir_processor_unary(type(f32), type(f32), apply1, asin);
}

doremir_processor_t doremir_processor_atan(doremir_type_t type)
{
    return doremir_processor_unary(type(f32), type(f32), apply1, atan);
}

doremir_processor_t doremir_processor_cos(doremir_type_t type)
{
    return doremir_processor_unary(type(f32), type(f32), apply1, cos);
}

doremir_processor_t doremir_processor_sin(doremir_type_t type)
{
    return doremir_processor_unary(type(f32), type(f32), apply1, sin);
}

doremir_processor_t doremir_processor_tan(doremir_type_t type)
{
    return doremir_processor_unary(type(f32), type(f32), apply1, tan);
}

doremir_processor_t doremir_processor_exp(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_log(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_log10(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_pow(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_sqrt(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_abs(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_min(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_max(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_fmod(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_remainder(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_floor(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_ceil(doremir_type_t type)
{
    assert(false && "Not implemented");
}

doremir_processor_t doremir_processor_rint(doremir_type_t type)
{
    assert(false && "Not implemented");
}




