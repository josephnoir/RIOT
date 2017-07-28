/*
 * Copyright (C) 2017 Hamburg University of Applied Sciences (HAW)
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup cpp11-compat
 * @{
 *
 * @file
 * @brief  C++11 chrono drop in replacement that adds the function now based on
 *         xtimer/timex
 *
 * @author Raphael Hiesgen <raphael.hiesgen (at) haw-hamburg.de>
 *
 * @}
 */

#include "riot/chrono.hpp"

using namespace std;

namespace riot {
namespace chrono {


// #### system_clock ####
/*
system_clock::time_point system_clock::now() {
  return time_point{};
}

timex_t system_clock::to_time_t(const time_point& t) noexcept {
  return timex_t{};
}

system_clock::time_point system_clock::from_time_t(timex_t t) noexcept {
  return time_point{};
}
*/

} // namespace chrono
} // namespace riot

