#include "quantize_test.h"
#include "quantum_copy_machine_test.h"

int main(int argc, char *argv[]) {
  test2x2();
  testNeighborhood2x2();
  test_read_png();
  return 0;
}