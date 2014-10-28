
#include <string>
#include <cstdio>

#include "mutex.hpp"
#include "thread.hpp"
#include "condition_variable.hpp"

using namespace std;
using namespace caf;

/* example from http://en.cppreference.com/w/cpp/thread/condition_variable */
int main() {
  printf("\n************ RIOT and C++ threads ***********\n");

  mutex m;
  condition_variable cv;
  string data;
  bool ready = false;
  bool processed = false;
 
  thread worker([&] {
    // Wait until main() sends data
    unique_lock<mutex> lk(m);
    cv.wait(lk, [&ready]{return ready;});
 
    // after the wait, we own the lock.
    printf("Worker thread is processing data\n");
    data += " after processing";
 
    // Send data back to main()
    processed = true;
    printf("Worker thread signals data processing completed\n");
 
    // Manual unlocking is done before notifying, to avoid
    // that the waiting thread gets blocked again.
    lk.unlock();
    cv.notify_one();
  });
 
  data = "Example data";
  // send data to the worker thread
  {
    lock_guard<mutex> lk(m);
    ready = true;
    printf("main() signals data ready for processing\n");
  }
  cv.notify_one();

  // wait for the worker
  {
    unique_lock<mutex> lk(m);
    cv.wait(lk, [&processed]{return processed;});
  }
  printf("Back in main(), data = %s\n", data.c_str());;

  printf("Bye, bye.\n");
  printf("*********************************************\n");

  return 0;
}
