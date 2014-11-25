
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
  printf("\n************ C++ mutex test ***********\n");

  printf("Lock and unlock ... \n");
  {
    mutex m;
    int resource = 0;
    auto f = [&m,&resource]{
      for (int i = 0; i < 3; ++i) {
        m.lock();
        ++resource;
        this_thread::sleep_for(chrono::milliseconds(100));
        m.unlock();
      }
    };
    assert(resource == 0);
    auto start = std::chrono::system_clock::now();
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    assert(resource == 6);
    auto duration = std::chrono::duration_cast<chrono::milliseconds>(std::chrono::system_clock::now() - start);
    assert(duration.count() >= 600);
  }
  printf("Done\n");

  printf("Try_lock ...\n");
  {
    mutex m;
    m.lock();
    thread([&m]{
      auto res = m.try_lock();
      assert(res == false);
    }).detach();
    m.unlock();
  }

  {
    mutex m;
    thread([&m]{
      auto res = m.try_lock();
      assert(res == true);
      m.unlock();
    }).detach();
  }
  printf("Done\n");

  printf("Bye, bye.\n");
  printf("*****************************************\n");

  return 0;
}
