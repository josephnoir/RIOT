/*
 * Copyright (C) 2015 Hamburg University of Applied Sciences (HAW)
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief test thread replacement header
 *
 * @author Raphael Hiesgen <raphael.hiesgen@haw-hamburg.de>
 *
 * @}
 */

#include <string>
#include <cstdio>
#include <cassert>
#include <system_error>

#include "riot/chrono.hpp"

using namespace std;
using namespace riot;

/* http://en.cppreference.com/w/cpp/header/chrono */
int main() {
  puts("\n************ C++ riot::chrono test ***********");

  using shakes = riot::chrono::duration<int, std::ratio<1, 100000000>>;
  using jiffies = riot::chrono::duration<int, std::centi>;

  riot::chrono::seconds sec(1);

  assert(riot::chrono::microseconds(sec).count() == 1000000);
  assert(shakes(sec).count() == 100000000);
  assert(jiffies(sec).count() == 100);
  assert(shakes(sec) == jiffies(sec));

  printf("1 second is:\n");
  // integer scale conversion with no precision loss: no cast
  printf("%lld microseconds\n%d shakes\n%d jiffies\n",
         riot::chrono::microseconds(sec).count(),
         shakes(sec).count(), jiffies(sec).count());

/*
          THERE IS NO FLOATING STUFF ON THE BOARD I USED FOR TESTING!
  // using microfortnights = riot::chrono::duration<float, std::ratio<12096,10000>>;
  // using nanocenturies = riot::chrono::duration<float, std::ratio<3155,1000>>;
  // integer scale conversion with precision loss: requires a cast
  printf("%ld minutes\n",
         static_cast<int32_t>(riot::chrono::duration_cast<riot::chrono::minutes>(sec).count()));

  // floating-point scale conversion: no cast
  printf("%ld microfortnights\n%ld nanocenturies\n",
         static_cast<int32_t>(microfortnights(sec).count()),
         static_cast<int32_t>(nanocenturies(sec).count()));

  float foo = 1.337;
  printf("a float = %f\n", foo);
*/

  puts("Bye, bye.");
  puts("******************************************");

  return 0;
}
