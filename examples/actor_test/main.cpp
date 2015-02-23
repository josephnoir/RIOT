/******************************************************************************\
 * This example is an implementation of the classical Dining Philosophers     *
 * exercise using only libcaf's event-based actor implementation.             *
\ ******************************************************************************/

#include <map>
#include <vector>
#include <chrono>
#include <sstream>
#include <iostream>

#include "caf/all.hpp"
#include "caf/chrono.hpp"

#include "ps.h"

using std::chrono::seconds;

using namespace std;
using namespace caf;

namespace {

// either taken by a philosopher or available
void chopstick(event_based_actor* self) {
  self->become(
    on(atom("take"), arg_match) >> [=](const actor& philos) {
      // tell philosopher it took this chopstick
      self->send(philos, atom("taken"), self);
      // await 'put' message and reject other 'take' messages
      self->become(
        // allows us to return to the previous behavior
        keep_behavior,
        on(atom("take"), arg_match) >> [=](const actor& other) {
          self->send(other, atom("busy"), self);
        },
        on(atom("put"), philos) >> [=] {
          // return to previous behaivor, i.e., await next 'take'
          self->unbecome();
        }
      );
    }
  );
}

class philosopher : public event_based_actor {
 public:
  philosopher(const std::string& n)
      : name(n) {
    // a philosopher that receives {eat} stops thinking and becomes hungry
    thinking = (
      on(atom("eat")) >> [=] {
        become(hungry);
        send(left, atom("take"), this);
        send(right, atom("take"), this);
      }
    );
    // wait for the first answer of a chopstick
    hungry = (
      on(atom("taken"), left) >> [=] {
        become(waiting_for(right));
      },
      on(atom("taken"), right) >> [=] {
        become(waiting_for(left));
      },
      on<atom("busy"), actor>() >> [=] {
        become(denied);
      }
    );
    // philosopher was not able to obtain the first chopstick
    denied = (
      on(atom("taken"), arg_match) >> [=](const actor& ptr) {
        send(ptr, atom("put"), this);
        send(this, atom("eat"));
        become(thinking);
      },
      on<atom("busy"), actor>() >> [=] {
        send(this, atom("eat"));
        become(thinking);
      }
    );
    // philosopher obtained both chopstick and eats (for five seconds)
    eating = (
      on(atom("think")) >> [=] {
        send(left, atom("put"), this);
        send(right, atom("put"), this);
        delayed_send(this, seconds(5), atom("eat"));
        printf("%s puts down his chopsticks and starts to think \n", name.c_str());
        become(thinking);
      }
    );
  }

 protected:
  behavior make_behavior() override {
    // start thinking
    // send(this, atom("think"));
    // philosophers start to think after receiving {think}
    return (
      on(atom("think")) >> [=] {
        printf("%s starts to think\n", name.c_str());
        delayed_send(this, seconds(5), atom("eat"));
        become(thinking);
      }
    );
  }

 private:
  // wait for second chopstick
  behavior waiting_for(const actor& what) {
    return {
      on(atom("taken"), what) >> [=] {
        printf("%s has picked up chopsticks with IDs %lu and %lu and starts to eat\n",
               name.c_str(), left->id(), right->id());
        // eat some time
        delayed_send(this, seconds(5), atom("think"));
        become(eating);
      },
      on(atom("busy"), what) >> [=] {
        send((what == left) ? right : left, atom("put"), this);
        send(this, atom("eat"));
        become(thinking);
      }
    };
  }

  std::string name;   // the name of this philosopher
  actor       left;   // left chopstick
  actor       right;  // right chopstick
  behavior    thinking;
  behavior    hungry; // tries to take chopsticks
  behavior    denied; // could not get chopsticks
  behavior    eating; // wait for some time, then go thinking again
};

} // namespace <anonymous>

int main(int, char**) {
  for (size_t i = 0; i < 100; ++i) {
    spawn<philosopher,detached>("a_philosopher");
    printf("thread running:\n");
    thread_print_all();
    this_thread::sleep_for(chrono::seconds(2));
    printf("\n\n");
  }
  await_all_actors_done();
  shutdown();
  return 0;
}
