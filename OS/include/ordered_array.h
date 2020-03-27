#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H 0

#include "common.h"

typedef void* type_t;
typedef sint8_t (*lessthan_predicate_t)(type_t,type_t);
typedef struct
{
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern sint8_t standard_lessthan_predicate(type_t a, type_t b);
extern ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than);
extern ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);
extern void destroy_ordered_array(ordered_array_t *array);
extern void insert_ordered_array(type_t item, ordered_array_t *array);
extern type_t lookup_ordered_array(uint32_t i, ordered_array_t *array);
extern void remove_ordered_array(uint32_t i, ordered_array_t *array);

#ifdef __cplusplus
}
#endif

#endif // ORDERED_ARRAY_H
