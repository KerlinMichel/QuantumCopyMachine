#include <stdlib.h>

#include "quantum_copy_machine.h"

int *nd_array(size_t num_dimensions, size_t dimension_sizes[])
{
    size_t size = sizeof(int);

    // repeatly multiply the size by the number of dimensions
    for (size_t n = 0; n < num_dimensions; ++n)
        size *= dimension_sizes[n];

    return malloc(size);
}

int nd_index(size_t num_dimensions, size_t index[], size_t dimension_sizes[])
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