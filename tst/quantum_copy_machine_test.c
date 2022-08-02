#include <assert.h>
#include <stdio.h>

#include "quantum_copy_machine.h"

void test2x2()
{
    size_t dim[2] = { 2, 2 };
    int *arr = nd_array(2, dim);

    size_t idx[2] = { 0, 0 };
    int i = nd_index(2, idx, dim);

    assert(i == 0);

    size_t idx2[2] = { 1, 1 };
    i = nd_index(2, idx2, dim);

    assert(i == 3);
}

int main(int argc, char *argv[]) {
  test2x2();
  return 0;
}