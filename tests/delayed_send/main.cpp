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
#include "caf/chrono.hpp"

using namespace std;
using namespace caf;

namespace {
  constexpr unsigned timeout = 500000;
}

behavior ping(event_based_actor* self, int group) {
  return {
    on(atom("ping"), arg_match) >> [=](const actor& pong_actor, int cnt) {
      printf("ping:%d\n", group);
      self->delayed_send(pong_actor, chrono::microseconds(timeout), atom("pong"), cnt + 1);
    },
    others() >> [] {
      printf("[ping] received unknown message\n");
    }
  };
}

behavior pong(event_based_actor* self, const actor& pung_actor, int group) {
  self->send(pung_actor, atom("pung"), self, 0);
  return {
    on(atom("pong"), arg_match) >> [=](int cnt) {
      printf("pong:%d\n", group);
      self->delayed_send(pung_actor, chrono::microseconds(timeout), atom("pung"), self, cnt);
    },
    on(atom("poke"), arg_match) >> [=] (const actor& dude) {
      self->delayed_send(dude, chrono::microseconds(timeout), atom("poke"));
    },
    others() >> [] {
      printf("[pong] received unknown message\n");
    }
  };
}

behavior pung(event_based_actor* self, const actor& pang_actor, int group) {
  return {
    on(atom("pung"), arg_match) >> [=] (const actor& pong_actor, int cnt) {
      printf("pung:%d\n", group);
      self->delayed_send(pang_actor, chrono::microseconds(timeout), atom("pang"), pong_actor, cnt);
    },
    others() >> [] {
      printf("[pung] received unknown message\n");
    }
  };
}

behavior pang(event_based_actor* self, const actor& ping_actor, int group) {
  return {
    on(atom("pang"), arg_match) >> [=] (const actor& pong_actor, int cnt) {
      printf("pang:%d\n", group);
      self->delayed_send(ping_actor, chrono::microseconds(timeout), atom("ping"), pong_actor, cnt);
    },
    others() >> [] {
      printf("[pang] received unknown message\n");
    }
  };
}

int main() {
  printf("\n************ RIOT and CAF demo ***********\n");

  auto a = spawn<detached>(ping, 1);
  auto b = spawn<detached>(pang, a, 1);
  auto c = spawn<detached>(pung, b, 1);
  auto d = spawn<detached>(pong, c, 1);

  auto f = spawn<detached>(ping, 2);
  auto g = spawn<detached>(pang, f, 2);
  auto h = spawn<detached>(pung, g, 2);
  auto i = spawn<detached>(pong, h, 2);

  await_all_actors_done();
  shutdown();

  printf("******************************************\n");
  return 0;
}
