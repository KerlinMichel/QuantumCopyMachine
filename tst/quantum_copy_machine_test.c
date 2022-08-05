#include <assert.h>
#include <stdio.h>

#include "quantum_copy_machine.h"

void test2x2()
{
    size_t dim[2] = { 2, 2 };
    int *arr = nd_array(2, dim);

    size_t idx[2] = { 0, 0 };
    int i = flatten_nd_index(2, idx, dim);

    assert(i == 0);

    size_t idx2[2] = { 1, 1 };
    i = flatten_nd_index(2, idx2, dim);

    assert(i == 3);
}

void testNeighborhood2x2()
{
    size_t dim[2] = { 2, 2 };
    int num_index_offsets = 3;

    size_t **neighborhood_index_offsets = (size_t **)malloc(num_index_offsets*sizeof(size_t*));
    for (int i = 0; i < 3; i++)
        neighborhood_index_offsets[i] = malloc(2*sizeof(size_t));

    neighborhood_index_offsets[0][0] = 1;
    neighborhood_index_offsets[0][1] = 0;

    neighborhood_index_offsets[1][0] = 0;
    neighborhood_index_offsets[1][1] = 1;

    neighborhood_index_offsets[2][0] = 1;
    neighborhood_index_offsets[2][1] = 1;

    int *n = neighborhood(neighborhood_index_offsets, 3, 2, dim);

    assert(n[0] == 1);
    assert(n[1] == 2);
    assert(n[2] == 3);
}

int main(int argc, char *argv[]) {
  test2x2();
  testNeighborhood2x2();
  return 0;
}