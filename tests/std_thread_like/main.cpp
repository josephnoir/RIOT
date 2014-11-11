
#include <string>
#include <cstdio>
#include <cassert>
#include <system_error>

#include "caf/mutex.hpp"
#include "caf/thread.hpp"
#include "caf/condition_variable.hpp"

using namespace std;
using namespace caf;

/* example from http://en.cppreference.com/w/cpp/thread/condition_variable */
int main() {
  printf("\n************ RIOT and C++ thread test ***********\n");
 
  printf("Creating one thread\n");
  {
    thread t([] {
      //nop
    });
    try {
      t.join();
    } catch (const std::system_error& e) {
      assert(false);
    }
  }
  printf("Done\n");

  printf("Testing join on 'finished' thread\n");
  {
    thread t;
    assert(t.joinable() == 0);

    t = thread([]{
      printf("hi\n");
    });
    assert(t.joinable() == 1);

    try {
      t.join();
    } catch (const std::system_error& e) {
      printf("e.what(): %s\n", e.what());
    }
  }
  printf("Done\n");

  printf("Testing join on 'running' thread\n");
  {
    mutex m;
    thread t1,t2;
    condition_variable cv;
    assert(t1.joinable() == 0);
    assert(t2.joinable() == 0);
    printf("Creating first thread\n");
    t1 = thread([&m,&cv]{
      unique_lock<mutex> lk(m);
      cv.wait(lk);
    });
    assert(t1.joinable() == 1);
    printf("Creating second thread\n");
    t2 = thread([&t1]{
      try {
        printf("Joining first thread\n");
        t1.join();
      } catch (const std::system_error& e) {
        assert(false);
      }
    });
    assert(t2.joinable() == 1);
    printf("Notifying condition\n");
    cv.notify_one();
    try {
      printf("Joining second thread\n");
      t2.join();
    } catch (const std::system_error& e) {
      assert(false);
    }
    assert(t1.joinable() == 0);
    assert(t2.joinable() == 0);
  }
  printf("Done\n");
  
  printf("Bye, bye.\n");
  printf("*********************************************\n");

  return 0;
}
