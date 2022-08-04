#ifndef __quantum_copy_machine_H__
#define __quantum_copy_machine_H__

#include <stdlib.h>

int *nd_array(size_t num_dimensions, size_t dimension_sizes[]);

int nd_index(size_t num_dimensions, size_t nd_index[], size_t dimension_sizes[]);

int *neighborhood(size_t **index_offsets, int num_index_offsets, int num_dimensions, size_t dimension_sizes[]);

#endif
