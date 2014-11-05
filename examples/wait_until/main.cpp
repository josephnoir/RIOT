#include <iostream>
#include <atomic>
#include <chrono>

#include "thread.hpp"
#include "condition_variable.hpp"

using namespace std;
using namespace caf;

condition_variable cv;
mutex cv_m;
atomic<int> i = ATOMIC_VAR_INIT(0);
atomic<int> j = ATOMIC_VAR_INIT(0);

void waits(int idx) {
  unique_lock<mutex> lk(cv_m);
  auto now = chrono::system_clock::now();
  if(cv.wait_until(lk, now + chrono::milliseconds(idx*100), [](){return i == 1;})) {
    printf("[waits] Thread %d finished waiting. i == %d\n", idx, i.load());
  } else {
    printf("[waits] Thread %d timed out. i == %d\n", idx, i.load());
  }
  ++j;
  cv.notify_all();
}

void signals() {
  this_thread::sleep_for(chrono::milliseconds(120));
  printf("[signals] Making noise...\n");
  cv.notify_all();
  this_thread::sleep_for(chrono::milliseconds(100));
  i = 1;
  printf("[signals] Making noise again...\n");
  cv.notify_all();
}

int main() {
  thread(waits, 1).detach();
  thread(waits, 2).detach();
  thread(waits, 3).detach();
  thread(signals).detach();
  printf("[main] waiting for threads\n");
  unique_lock<mutex> lk(cv_m);
  auto now = chrono::system_clock::now();
  if (cv.wait_until(lk, now + chrono::milliseconds(500), [] { return j==3; })) {
    printf("[main] all threads done\n");
  } else {
    printf("[main] let's kill some threads\n");
  }
}
