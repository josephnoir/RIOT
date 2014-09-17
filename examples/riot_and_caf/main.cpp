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

/* main */
int main() {
  printf("\n************ RIOT and CAF demo ***********\n");
  auto bttn = spawn<detached>(button, "The Button");
  auto dude = spawn<detached>(fool, "The Fool");
  
  anon_send(dude, atom("find"), bttn);

  await_all_actors_done();
  shutdown();

  printf("\n******************************************\n");
  return 0;
}
