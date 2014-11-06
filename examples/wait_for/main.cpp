#include <atomic>
#include <chrono>
#include <iostream>

#include "thread.hpp"
#include "condition_variable.hpp"

using namespace std;

/*
caf::condition_variable cv;
caf::mutex cv_m;
atomic<int> i = ATOMIC_VAR_INIT(0);
atomic<int> j = ATOMIC_VAR_INIT(0);

void waits(int idx) {
  caf::unique_lock<caf::mutex> lk(cv_m);
  if(cv.wait_for(lk, chrono::milliseconds(idx*100), [](){return i == 1;})) {
    printf("[waits] Thread %d finished waiting. i == %d\n", idx, i.load());
  } else {
    printf("[waits] Thread %d timed out. i == %d\n", idx, i.load());
  }
  ++j;
  cv.notify_all();
}

void signals() {
  caf::this_thread::sleep_for(chrono::milliseconds(120));
  printf("[signals] Making noise...\n");
  cv.notify_all();
  caf::this_thread::sleep_for(chrono::milliseconds(100));
  i = 1;
  printf("[signals] Making noise again...\n");
  cv.notify_all();
}
*/

int main() {
/*  caf::thread(waits, 1).detach();
  caf::thread(waits, 2).detach();
  caf::thread(waits, 3).detach();
  caf::thread(signals).detach();
  printf("[main] waiting for threads\n");
  caf::unique_lock<caf::mutex> lk(cv_m);
  auto now = chrono::system_clock::now();
  if (cv.wait_until(lk, now + chrono::milliseconds(5000), [] { return j==3; })) {
    printf("[main] all threads done\n");
  } else {
    printf("[main] let's kill some threads\n");
  }*/

  caf::mutex mtx;
  caf::condition_variable cv;
  caf::unique_lock<caf::mutex> lk(mtx);
  auto to = cv.wait_for(lk, chrono::milliseconds(100));
  if (to == caf::cv_status::timeout) {
    printf("[main] timeout\n");
  } else if (to == caf::cv_status::no_timeout) {
    printf("[main] no timeout\n");
  } else {
    printf("[main] wtf\n");
  }
}
