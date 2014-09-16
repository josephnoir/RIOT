
#include <cstdio>

#include "thread.hpp"
//#include "mutex.hpp"

using namespace std;

//void* func(void*) {
//  printf("Hallo\n");
//}

/* main */
int main() {
  printf("\n************ RIOT and C++ threads ***********\n");

//  thread t(func);
  caf::thread t([] {
    printf("Hello\n");
  });

  printf("Bye, bye.\n");
  printf("*********************************************\n");

  return 0;
}
