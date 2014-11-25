
#include <cstdio>

#include "caf/mutex.hpp"
#include "caf/chrono.hpp"
#include "caf/thread.hpp"
#include "caf/condition_variable.hpp"

using namespace std;
using namespace caf;

namespace {
  constexpr unsigned timeout = 1000;
}

/* main */
int main() {
  printf("Starting test, simple incrementing timer\n");
  for (int i = 0; i < 1000000; ++i) {
    printf("[%06d]\n", i);
    mutex m;
    condition_variable cv;
    unique_lock<mutex> lk(m);
    auto time = caf::now() += chrono::microseconds(timeout);
    cv.wait_until(lk, time);
  }
  printf("Done\n");
  return 0;
}

