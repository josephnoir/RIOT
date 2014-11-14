
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

/* http://en.cppreference.com/w/cpp/thread/thread */
int main() {
  printf("\n************ RIOT and C++ thread test ***********\n");

  assert(sched_num_threads == 2); // main + idle
 
  printf("Creating one thread and passing an argument ...\n");
  {
    constexpr int i = 3;
    thread t([](const int j) {
      assert(j == i);
    }, i);
    try {
      t.join();
    } catch (const std::system_error& e) {
      assert(false);
    }
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Creating detached thread ...\n");
  {
    thread t([] {
      //nop
    });
    assert(t.joinable() == 1);
    t.detach();
    assert(t.joinable() == 0);
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Join on 'finished' thread ...\n");
  {
    thread t;
    assert(t.joinable() == 0);
    t = thread([]{
      // nop
    });
    assert(t.joinable() == 1);
    try {
      t.join();
    } catch (const std::system_error& e) {
      printf("e.what(): %s\n", e.what());
    }
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Join on 'running' thread ...\n");
  {
    mutex m;
    thread t1,t2;
    condition_variable cv;
    assert(t1.joinable() == 0);
    assert(t2.joinable() == 0);
    t1 = thread([&m,&cv]{
      unique_lock<mutex> lk(m);
      cv.wait(lk);
    });
    assert(t1.joinable() == 1);
    t2 = thread([&t1]{
      try {
        t1.join();
      } catch (const std::system_error& e) {
        assert(false);
      }
    });
    assert(t2.joinable() == 1);
    cv.notify_one();
    try {
      t2.join();
    } catch (const std::system_error& e) {
      assert(false);
    }
    assert(t1.joinable() == 0);
    assert(t2.joinable() == 0);
  }
  printf("Done\n");

  assert(sched_num_threads == 2);
  
  printf("Testing sleep_for ...\n");
  {
    timex_t before, after;
    vtimer_now(&before);
    this_thread::sleep_for(chrono::seconds(1));
    vtimer_now(&after);
    auto diff = timex_sub(after,before);
    assert(diff.seconds >= 1);
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Swapping two threads ...\n");
  {
    thread t1([]{
      // nop
    });
    thread t2([]{
      // nop
    });
    auto t1_old = t1.get_id();
    auto t2_old = t2.get_id();
    t1.swap(t2);
    assert(t1_old == t2.get_id());
    assert(t2_old == t1.get_id());
    t1.join();
    t2.join();
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Move constructor ... \n");
  {
    thread t1([]{
      //nop
    });
    thread t2(move(t1));
    assert(t1.joinable() == 0);
    assert(t2.joinable() == 1);
    t2.join();
  }
  printf("Done\n");

  assert(sched_num_threads == 2);

  printf("Bye, bye.\n");
  printf("*********************************************\n");

  return 0;
}
