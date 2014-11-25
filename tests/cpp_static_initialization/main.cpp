
#include <cstdio>
#include <cassert>

using namespace std;

namespace {
  constexpr int val = 2;
}

struct my_struct {
  my_struct() : data(val) { }
  int data;
};

static my_struct ms;

int main() {
  printf("\n************ C++ const initialization test ***********\n");

  assert(ms.data == val);

  printf("Done.\n");
  printf("********************************************************\n");

  return 0;
}
