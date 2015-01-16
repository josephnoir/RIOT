
#include <cstdio>
#include <string>

#include "caf/mutex.hpp"
#include "caf/chrono.hpp"
#include "caf/thread.hpp"
#include "caf/condition_variable.hpp"

using namespace std;
using namespace caf;

mutex mtx;
condition_variable cv;
bool ready;
string shared;

void fluffy() {
  unique_lock<mutex> lk(mtx);
  cv.wait(lk, []{return ready;});
  printf("[fluffy] has been %s\n", shared.c_str());
}

int main() {
  printf("\n************ RIOT and Thread ***********\n");
  ready = false;
  thread t(fluffy);
  shared = "poked";
  ready = true;
  cv.notify_one();
  t.join();
  printf("******************************************\n");
  return 0;
}

