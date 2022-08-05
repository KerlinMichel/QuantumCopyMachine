#include <stdlib.h>

#include "quantum_copy_machine.h"

/*
 * nd_array: creates a flat (1 dimensional) int array that can store each entry in a multidimensional array
 */
int *nd_array(size_t num_dimensions, size_t dimension_sizes[])
{
    size_t size = sizeof(int);

    // repeatly multiply the size by the number of dimensions
    for (size_t n = 0; n < num_dimensions; ++n)
        size *= dimension_sizes[n];

    return malloc(size);
}

/*
 * flatten_nd_index: converts a multi dimensional index to a flat index representation (see nd_array())
 */
int flatten_nd_index(size_t num_dimensions, size_t index[], size_t dimension_sizes[])
{
    if (num_dimensions == 0)
        return 0;

    int index_ = index[0];

    for (size_t n = 1; n < num_dimensions; ++n) {
        size_t d = 1;

        // multiply current index by the multiplication of max values for previous dimensions
        for (size_t m = n; m > 0; --m)
        {
            d = d * dimension_sizes[m-1];
        }

        index_ = index_ + (d * index[n]);
    }

    return index_;
}

struct neighbor_clause {
    int n_0_state; // neighbor 0 state
    int n_1_state; // neighbor 1 state
    int index_offset; // neighbor_0_index + index_offset = neighbor_1_index
};

int *neighborhood(size_t **index_offsets, int num_index_offsets, int num_dimensions, size_t dimension_sizes[])
{
    int *neighborhood = (int*)malloc(num_index_offsets*sizeof(int));

    for(int i = 0; i < num_index_offsets; i++)
    {
        neighborhood[i] = flatten_nd_index(num_dimensions, (size_t *)index_offsets[i],  dimension_sizes);
    }

    return neighborhood;
}