#include <string>
#include <cstdio>

#include "caf/all.hpp"
#include "caf/chrono.hpp"

using namespace std;
using namespace caf;

behavior fluffy(event_based_actor* self) {
  return {
    on(atom("poke"), arg_match) >> [=](int cnt) {
      printf("[%06d]\n", cnt);
      self->quit();
    }
  };
}

int main() {
  printf("\n************ RIOT and CAF ***********\n");
  auto f = spawn<detached>(fluffy);
  anon_send(f, atom("poke"), 0);
  await_all_actors_done();
  shutdown();
  printf("***************************************\n");
  return 0;
}