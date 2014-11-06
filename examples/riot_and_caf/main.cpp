/*
 * Copyright (C) 2014 Hamburg University of Applied Sciences (HAW)
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file        main.cpp
 * @brief       Demonstration of ...
 * @author      Martin Landsmann <martin.landsmann@haw-hamburg.de>
 * @author      Raphael Hiesgen <rapheal.hiesgen@haw-hamburg.de>
 */

#include <string>
#include <cstdio>

#include "caf/all.hpp"

using namespace std;
using namespace caf;
/*
behavior button(event_based_actor* self, const string& identifier) {
  return {
    on(atom("press")) >> [=] {
      printf("%s: You fool! ...\n", identifier.c_str());
      return atom("quit");
    },
    on(atom("quit")) >> [=] {
      self->quit();
      //return atom("quit");
    }
  };
}

behavior fool(event_based_actor* self, const string& identifier) {
  return {
    on(atom("find"), arg_match) >> [=] (const actor& thing) {
      printf("%s: Oh a button, lets press it.\n", identifier.c_str());
      self->send(thing, atom("press"));
    },
    on(atom("quit")) >> [=] {
      self->quit();
      return atom("quit");
    }
  };
}
*/

behavior fluffy(event_based_actor* self) {
  printf("blaaaa\n");
  return {
    on(atom("poke"), arg_match) >> [=](const actor& other) {
      printf("quiek!\n");
      self->send(other, atom("blub"));
      self->quit();
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
  //auto f = spawn<fluffy,detached>();
  {
    scoped_actor self;
    self->send(f, atom("poke"), self);
    // self->send(f, atom("quit"));
    self->receive(
      others() >> [] {
        printf("blubb\n");
      }
    );
  }


/*
  auto bttn = spawn<detached>(button, "The Button");
  auto dude = spawn<detached>(fool, "The Fool");
  anon_send(dude, atom("find"), bttn);
*/
/*
  {
    scoped_actor self;
    //self->send(self, atom("blub"));
    spawn(sender, self);
    self->receive(
      others() >> [] {
        printf("blubb\n");
      }
    );
  }
*/
  await_all_actors_done();
  shutdown();

  printf("******************************************\n");
  return 0;
}
