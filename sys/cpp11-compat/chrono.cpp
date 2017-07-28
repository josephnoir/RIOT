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

system_clock::time_point system_clock::now() noexcept {
  timex_t tp;
  xtimer_now_timex(&tp);
  rep msecs = tp.microseconds + tp.seconds * microsecs_in_sec;
  return time_point{duration{msecs}};
}

timex_t system_clock::to_time_t(const time_point& t) noexcept {
  auto d = t.time_since_epoch();
  auto s = duration_cast<seconds>(d);
  auto m = duration_cast<microseconds>(duration_cast<microseconds>(d) - s);
  timex_t tp{static_cast<uint32_t>(s.count()),
             static_cast<uint32_t>(m.count())};
  return tp;
}

system_clock::time_point system_clock::from_time_t(timex_t t) noexcept {
  return time_point{seconds{t.seconds} + microseconds{t.microseconds}};
}

} // namespace chrono
} // namespace riot

