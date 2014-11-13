
#include <string>
#include <chrono>
#include <cstdio>
#include <cassert>
#include <system_error>

#include "caf/mutex.hpp"
#include "caf/thread.hpp"
#include "caf/condition_variable.hpp"

using namespace std;
using namespace caf;

/* http://en.cppreference.com/w/cpp/thread/mutex */
int main() {
  printf("\n************ RIOT and C++ condition_variable test ***********\n");

  printf("Wait with predicate and notify one ... \n");
  {
    mutex m;
    condition_variable cv;
    string data;
    bool ready = false;
    bool processed = false;
    thread worker([&]{
      unique_lock<mutex> lk(m);
      cv.wait(lk, [&ready]{return ready;});
      data += " after processing";
      processed = true;
      cv.notify_one();
    });
    data = "Example data";
    {
      lock_guard<mutex> lk(m);
      ready = true;
      cv.notify_one();
    }
    {
      unique_lock<mutex> lk(m);
      cv.wait(lk, [&processed]{return processed;});
    }
    string expected = "Example data after processing";
    assert(data == expected);
    worker.join();
  }
  printf("Done\n");

  printf("Wait and notify all ...\n");
  {
    mutex m;
    condition_variable cv;
    auto waits = [&m,&cv] {
      unique_lock<mutex> lk(m);
      cv.wait(lk);
    };
    thread t1(waits);
    thread t2(waits);
    thread t3(waits);
    thread t4(waits);
    thread([&m,&cv]{
      unique_lock<mutex> lk(m);
      cv.notify_all();
    }).detach();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
  }
  printf("Done\n");

  printf("Wait for ...\n");
  {
    using chrono::system_clock;
    constexpr unsigned timeout = 1000;
    mutex m;
    condition_variable cv;
    auto start = system_clock::now();
    unique_lock<mutex> lk(m);
    cv.wait_for(lk, chrono::milliseconds(timeout));
    auto diff = chrono::duration_cast<chrono::milliseconds>(system_clock::now() - start);
    assert(diff.count() >= timeout);
  }
  printf("Done\n");

  printf("Wait until ...\n");
  {
    using chrono::system_clock;
    constexpr unsigned timeout = 1000;
    mutex m;
    condition_variable cv;
    auto start = system_clock::now();
    auto tp = start + chrono::milliseconds(timeout);
    unique_lock<mutex> lk(m);
    cv.wait_until(lk, tp);
    auto diff = chrono::duration_cast<chrono::milliseconds>(system_clock::now() - start);
    assert(diff.count() >= timeout);
  }
  printf("Done\n");

  printf("Bye, bye.\n");
  printf("*********************************************\n");

  return 0;
}
