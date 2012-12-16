
#include <doremir/list.h>
#include <doremir/util.h>

doremir_ptr_t list_impl(doremir_id_t interface);

/*  List entry, end represented by null. */
struct Node
{
    size_t        count;
    struct Node*  next;
    doremir_ptr_t value;
};
typedef struct Node *NodeRef;

inline static NodeRef 
NewNode(doremir_ptr_t x, NodeRef xs)
{
    NodeRef node = malloc(sizeof(struct Node));
    node->count = 1;
    node->next  = xs;
    node->value = x;
    return node;
}

inline static NodeRef
TakeNode(NodeRef node)
{
    if (!node) return node;
    node->count++;
    return node;
}

inline static void
ReleaseNode (NodeRef node)
{
    if (!node) return;
    
    node->count--;
    if (node->count <= 0)
    {
        ReleaseNode(node->next);
        free(node);
    }
}             

struct _doremir_list_t
{
    doremir_impl_t  impl;
    struct Node*    node;
};

inline static doremir_list_t 
NewList(NodeRef node)
{
    doremir_list_t xs = doremir_new(list);
    xs->impl = &list_impl;    
    xs->node = node;
    return xs;
}

#define DeleteList doremir_delete



// --------------------------------------------------------------------------------

doremir_list_t doremir_list_empty()
{   
    doremir_list_t xs = NewList(NULL);
    return xs;
}

doremir_list_t doremir_list_single(doremir_ptr_t x)
{
    doremir_list_t xs = NewList(
        NewNode(x, NULL));
    return xs;
}

doremir_list_t doremir_list_cons(doremir_ptr_t x, doremir_list_t xs)
{
    doremir_list_t ys = NewList(
        NewNode(x, TakeNode(xs->node)));    
    return ys;
}

doremir_list_t doremir_list_snoc(doremir_ptr_t x, doremir_list_t xs)
{
    // doremir_list_t ys = NewList(NULL);    
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_append(doremir_list_t xs, doremir_list_t ys)
{
    // doremir_list_t zs = NewList(NULL);    
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_copy(doremir_list_t xs)
{
    doremir_list_t ys = NewList(TakeNode(xs->node));
    return ys;
}

void doremir_list_destroy(doremir_list_t xs)
{                      
    // TODO optionally map registered deleter (a la CoreFoundation?)
    ReleaseNode(xs->node);
    DeleteList(xs);
}

// --------------------------------------------------------------------------------

bool doremir_list_is_empty(doremir_list_t xs)
{
    return !xs->node;
}

int doremir_list_lenght(doremir_list_t xs)
{                                      
    int          count = 0;
    struct Node* node  = xs->node;
    while (node)
    {                     
        count++;
        node = node->next;
    }
    return count;
}

doremir_ptr_t doremir_list_head(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_tail(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_init(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_last(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_take(int n, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_drop(int n, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

bool doremir_list_is_elem(doremir_ptr_t x, doremir_list_t xs)
{
    assert(false && "Not implemented");
}


// --------------------------------------------------------------------------------

doremir_list_t doremir_list_reverse(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_sort(doremir_list_ord_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_find(doremir_pred_t p, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_filter(doremir_pred_t p, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

// bool doremir_list_any(doremir_pred_t p, doremir_list_t xs)
// {
//     assert(false && "Not implemented");
// }
// 
// bool doremir_list_all(doremir_pred_t p, doremir_list_t xs)
// {
//     assert(false && "Not implemented");
// }

// --------------------------------------------------------------------------------

doremir_list_t doremir_list_map(doremir_unary_t f, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_fold(doremir_binary_t f,
                                 doremir_ptr_t    z,
                                 doremir_list_t   xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_concat(doremir_list_list_list_t xss)
{
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_sum(doremir_list_t xs)
{
    // TODO type of 0?
    return doremir_list_fold(doremir_add, fint32(0), xs);
}

doremir_ptr_t doremir_list_product(doremir_list_t xs)
{
    // TODO type of 0?
    return doremir_list_fold(doremir_multiply, fint32(1), xs);
}

doremir_ptr_t doremir_list_maximum(doremir_list_t xs)
{
    // return doremir_list_fold(doremir_max, fint32(INT_MIN), xs);
    assert(false && "Not implemented");
}

doremir_ptr_t doremir_list_minimum(doremir_list_t xs)
{
    // return doremir_list_fold(doremir_min, fint32(INT_MAX), xs);
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_consd(doremir_ptr_t x, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_snocd(doremir_ptr_t x, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_reversed(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_sortd(doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_mapd(doremir_unary_t f, doremir_list_t xs)
{
    assert(false && "Not implemented");
}

doremir_list_t doremir_list_foldd(doremir_binary_t f,
                                  doremir_ptr_t    z,
                                  doremir_list_t   xs)
{
    assert(false && "Not implemented");
}






bool list_equal(doremir_ptr_t a, doremir_ptr_t b)
{
    assert(false && "Not implemented");
}

bool list_less_than(doremir_ptr_t a, doremir_ptr_t b)
{
    assert(false && "Not implemented");
}

bool list_greater_than(doremir_ptr_t a, doremir_ptr_t b)
{
    assert(false && "Not implemented");
}

doremir_ptr_t list_copy(doremir_ptr_t a)
{
    assert(false && "Not implemented");
}
void list_destroy(doremir_ptr_t a)
{
    assert(false && "Not implemented");
}

doremir_ptr_t list_impl(doremir_id_t interface)
{
    static doremir_equal_t list_equal_impl = { list_equal };
    static doremir_copy_t list_copy_impl = { list_copy };
    static doremir_destroy_t list_destroy_impl = { list_destroy };
    static doremir_order_t list_order_impl = { list_less_than, list_greater_than };

    switch (interface)
    {
    case doremir_equal_i:
        return &list_equal_impl;

    case doremir_order_i:
        return &list_order_impl;

    case doremir_copy_i:
        return &list_copy_impl;

    case doremir_destroy_i:
        return &list_destroy_impl;

    default:
        return NULL;
    }
}
