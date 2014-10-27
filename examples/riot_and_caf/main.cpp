/*
* Copyright (C) 2014 Hamburg University of Applied Sciences (HAW)
*
* This file is subject to the terms and conditions of the GNU Lesser
* General Public License v2.1. See the file LICENSE in the top level
* directory for more details.
*/

/**
* @file main.cpp
* @brief Demonstration of ...
* @author Martin Landsmann <martin.landsmann@haw-hamburg.de>
* @author Raphael Hiesgen <rapheal.hiesgen@haw-hamburg.de>
*/

#include <string>
#include <cstdio>
#include "caf/all.hpp"
#include "caf/chrono.hpp"

using namespace std;
using namespace caf;

behavior fluffy(event_based_actor* self) {
  printf("It's so fluffy!!1!\n");
  return {
    on(atom("poke"), arg_match) >> [=](int cnt) {
      printf("[%06d]\n", cnt);
      self->delayed_send(self, chrono::microseconds(1000), atom("poke"), cnt + 1);
    },
    others() >> [] {
      printf("Unknown message\n");
    }
  };
}

/*
class fluffy : public event_based_actor {
 public:
  fluffy() {
    printf("it's fluffy!\n");
  }

  behavior make_behavior() override {
    return {
      on(atom("poke"), arg_match) >> [=](const actor& other) {
        printf("quiek!\n");
        send(other, atom("blub"));
        //return atom("blub");
      }
    };
  }
};
*/

/*
void sender(event_based_actor* self, const actor& other) {
  self->send(other, atom("blub"));
}
*/

int main() {
  printf("\n************ RIOT and CAF demo ***********\n");
  auto f = spawn<detached>(fluffy);
  anon_send(f, atom("poke"), 0);
  await_all_actors_done();
  shutdown();
  printf("******************************************\n");
  return 0;
}
