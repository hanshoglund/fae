
#include <doremir.h>

#pragma GCC diagnostic ignored "-Wparentheses"

/*
    doremir_ptr_t

    v = value
    _ = anything

    Layout                                  Tag
    ===================================     ===

    bool
    _______v ________ ________ _____111     0x7
    int8
    vvvvvvvv ________ ________ _____110     0x6
    int16
    vvvvvvvv vvvvvvvv ________ _____101     0x5
    boxed int32
    vvvvvvvv vvvvvvvv vvvvvvvv vvvvv100     0x4
    boxed int64
    vvvvvvvv vvvvvvvv vvvvvvvv vvvvv011     0x3
    boxed float
    vvvvvvvv vvvvvvvv vvvvvvvv vvvvv010     0x2
    boxed double
    vvvvvvvv vvvvvvvv vvvvvvvv vvvvv001     0x1
    ptr
    vvvvvvvv vvvvvvvv vvvvvvvv vvvvv000     0x0

 */

int doremir_type(doremir_ptr_t a)
{
    return ((intptr_t) a) & 0x7;
}

char *doremir_type_str(doremir_ptr_t a)
{
    switch (doremir_type(a))
    {
    case 7:
        return "bool";
    case 6:
        return "int8";
    case 5:
        return "int16";
    case 4:
        return "int32";
    case 3:
        return "int64";
    case 2:
        return "float";
    case 1:
        return "double";
    case 0:
        return "ptr";
    default:
        return "unknown";
    }
}


// --------------------------------------------------------------------------------
// Wrapper functions
// --------------------------------------------------------------------------------

bool doremir_to_bool(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x7 && "Wrong type");
    return (p & ~0x7) >> 24;
}

doremir_ptr_t doremir_from_bool(bool a)
{
    return (doremir_ptr_t) (a << 24 & ~0x7 | 0x7);
}

// --------------------------------------------------------------------------------

int8_t doremir_to_int8(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x6 && "Wrong type");
    return (p & ~0x7) >> 24;
}

doremir_ptr_t doremir_from_int8(int8_t a)
{
    return (doremir_ptr_t) (a << 24 & ~0x7 | 0x6);
}

// --------------------------------------------------------------------------------

int16_t doremir_to_int16(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x5 && "Wrong type");
    return (p & ~0x7) >> 8;
}

doremir_ptr_t doremir_from_int16(int16_t a)
{
    return (doremir_ptr_t) (a << 8 & ~0x7 | 0x5);
}

// --------------------------------------------------------------------------------

int32_t doremir_to_int32(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x4 && "Wrong type");
    int32_t v = *((int32_t*) (p & ~0x7));
    free((int32_t*) (p & ~0x7));
    return v;
}

doremir_ptr_t doremir_from_int32(int32_t a)
{
    int32_t *p = malloc(sizeof(int32_t));
    *p = a;
    return (doremir_ptr_t) (((intptr_t) p) | 0x4);
}

// --------------------------------------------------------------------------------

int64_t doremir_to_int64(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x3 && "Wrong type");
    int64_t v = *((int64_t*) (p & ~0x7));
    free((int64_t*) (p & ~0x7));
    return v;
}

doremir_ptr_t doremir_from_int64(int64_t a)
{
    int64_t *p = malloc(sizeof(int64_t));
    *p = a;
    return (doremir_ptr_t) (((intptr_t) p) & ~0x7 | 0x3);
}

// --------------------------------------------------------------------------------

float doremir_to_float(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x2 && "Wrong type");
    float v = *((float*) (p & ~0x7));
    free((float*) (p & ~0x7));
    return v;
}

doremir_ptr_t doremir_from_float(float a)
{
    float *p = malloc(sizeof(float));
    *p = a;
    return (doremir_ptr_t) (((intptr_t) p) & ~0x7 | 0x2);
}

// --------------------------------------------------------------------------------

double doremir_to_double(doremir_ptr_t a)
{
    intptr_t p = (intptr_t) a;
    assert((p & 0x7) == 0x1 && "Wrong type");
    double v = *((double*) (p & ~0x7));
    free((double*) (p & ~0x7));
    return v;
}

doremir_ptr_t doremir_from_double(double a)
{
    double *p = malloc(sizeof(double));
    *p = a;
    return (doremir_ptr_t) (((intptr_t) p) & ~0x7 | 0x1);
}

// --------------------------------------------------------------------------------
// Generic functions
// --------------------------------------------------------------------------------


bool doremir_equal(doremir_ptr_t a, doremir_ptr_t b)
{
    return ((doremir_equal_t*) doremir_interface(doremir_equal_i, a))->equal(a, b);
}

doremir_ptr_t doremir_copy(doremir_ptr_t a)
{
    return ((doremir_copy_t*) doremir_interface(doremir_copy_i, a))->copy(a);
}

void doremir_destroy(doremir_ptr_t a)
{
    return ((doremir_destroy_t*) doremir_interface(doremir_destroy_i, a))->destroy(a);
}

doremir_ptr_t doremir_move(doremir_ptr_t a)
{
    return a;
}

// --------------------------------------------------------------------------------
// Wrapped implementations
// --------------------------------------------------------------------------------

/* Generates T_impl
 */
#define IMPLEMENT_WRAPPER(T) \
    doremir_ptr_t T##_impl(doremir_id_t interface)                                          \
    {                                                                                       \
        static doremir_equal_t   T##_equal_impl   =                                         \
            { T##_equal };                                                                  \
        static doremir_order_t   T##_order_impl   =                                         \
            { T##_less_than, T##_greater_than };                                            \
        static doremir_number_t  T##_number_impl  =                                         \
            { T##_add, T##_subtract, T##_multiply, T##_divide, T##_modulo, T##_absolute };  \
        static doremir_copy_t    T##_copy_impl    =                                         \
            { T##_copy };                                                                   \
        static doremir_destroy_t T##_destroy_impl =                                         \
            { T##_destroy };                                                                \
                                                                                            \
        switch (interface)                                                                  \
        {                                                                                   \
        case doremir_equal_i:                                                               \
            return &T##_equal_impl;                                                         \
        case doremir_order_i:                                                               \
            return &T##_order_impl;                                                         \
        case doremir_number_i:                                                              \
            return &T##_number_impl;                                                        \
        case doremir_copy_i:                                                                \
            return &T##_number_impl;                                                        \
        case doremir_destroy_i:                                                             \
            return &T##_number_impl;                                                        \
        default:                                                                            \
            return NULL;                                                                    \
        }                                                                                   \
    }

#define UNBOXED_WRAPPER_IMPL(T) \
    bool T##_equal(doremir_ptr_t a, doremir_ptr_t b)                                        \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) == doremir_to_##T(b));                    \
    }                                                                                       \
    bool T##_less_than(doremir_ptr_t a, doremir_ptr_t b)                                    \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) < doremir_to_##T(b));                     \
    }                                                                                       \
    bool T##_greater_than(doremir_ptr_t a, doremir_ptr_t b)                                 \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) > doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_add(doremir_ptr_t a, doremir_ptr_t b)                                 \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) + doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_subtract(doremir_ptr_t a, doremir_ptr_t b)                            \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) - doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_multiply(doremir_ptr_t a, doremir_ptr_t b)                            \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) * doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_divide(doremir_ptr_t a, doremir_ptr_t b)                              \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) / doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_modulo(doremir_ptr_t a, doremir_ptr_t b)                              \
    {                                                                                       \
        return doremir_from_##T(doremir_to_##T(a) % doremir_to_##T(b));                     \
    }                                                                                       \
    doremir_ptr_t T##_absolute(doremir_ptr_t a)                                             \
    {                                                                                       \
        return doremir_from_##T(abs(doremir_to_##T(a)));                                    \
    }                                                                                       \
    doremir_ptr_t T##_copy(doremir_ptr_t a)                                                 \
    {                                                                                       \
        return a;                                                                           \
    }                                                                                       \
    void T##_destroy(doremir_ptr_t a)                                                       \
    {                                                                                       \
        /* nothing to do */                                                                 \
    }

// bool bool_equal(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) == doremir_to_bool(b));
// }
// bool bool_less_than(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) < doremir_to_bool(b));
// }
// bool bool_greater_than(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) > doremir_to_bool(b));
// }
// doremir_ptr_t bool_add(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) + doremir_to_bool(b));
// }
// doremir_ptr_t bool_subtract(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) - doremir_to_bool(b));
// }
// doremir_ptr_t bool_multiply(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) * doremir_to_bool(b));
// }
// doremir_ptr_t bool_divide(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) / doremir_to_bool(b));
// }
// doremir_ptr_t bool_modulo(doremir_ptr_t a, doremir_ptr_t b)
// {
//     return doremir_from_bool(doremir_to_bool(a) % doremir_to_bool(b));
// }
// doremir_ptr_t bool_absolute(doremir_ptr_t a)
// {
//     return doremir_from_bool(abs(doremir_to_bool(a)));
// }
// doremir_ptr_t bool_copy(doremir_ptr_t a)
// {
//     return a;
// }
// void bool_destroy(doremir_ptr_t a)
// {
//     // nothing to do
// }

UNBOXED_WRAPPER_IMPL(bool);
UNBOXED_WRAPPER_IMPL(int8);
UNBOXED_WRAPPER_IMPL(int16);

IMPLEMENT_WRAPPER(bool);
IMPLEMENT_WRAPPER(int8);
IMPLEMENT_WRAPPER(int16);
// IMPLEMENT_WRAPPER(int32);
// IMPLEMENT_WRAPPER(int64);
// IMPLEMENT_WRAPPER(float);
// IMPLEMENT_WRAPPER(double);


doremir_ptr_t doremir_interface(doremir_id_t type, doremir_ptr_t pointer)
{
    switch (doremir_type(pointer))
    {
    case 7:
        return bool_impl(type);
    case 6:
        return int8_impl(type);
    case 5:
        return int16_impl(type);
    // case 4:
    //     return int32_impl(type);
    // case 3:
    //     return int64_impl(type);
    // case 2:
    //     return float_impl(type);
    // case 1:
    //     return double_impl(type);
    default:
        return ((doremir_impl_t) pointer) (type);
    }
}
