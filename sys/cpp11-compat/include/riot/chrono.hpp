/*
 * Copyright (C) 2015 Hamburg University of Applied Sciences (HAW)
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
 * @see    <a href="http://en.cppreference.com/w/cpp/header/chrono">
 *           std::chrono, defined in header chrono
 *         </a> and
 *         <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm">
 *           sc22/wg21/docs/papers/2008/n2661
 *         </a>
 *
 * @author Raphael Hiesgen <raphael.hiesgen (at) haw-hamburg.de>
 *
 * @}
 */

#ifndef RIOT_CHRONO_HPP
#define RIOT_CHRONO_HPP

#include <ratio>
#include <limits>
#include <algorithm>
#include <type_traits>

// for static gcd / lcm
#include <cstdint>

#include "time.h"
#include "xtimer.h"

namespace riot {
namespace chrono {

// TODO: move this to a utility class
namespace detail {

template <intmax_t A, intmax_t B>
struct static_gcd {
  static constexpr intmax_t value = static_gcd<B, A % B>::value;
};

template <intmax_t A>
struct static_gcd<A,0> {
  static constexpr intmax_t value = A;
};

template <>
struct static_gcd<0,0> {
  static constexpr intmax_t value = 1;
};

template<intmax_t A, intmax_t B>
struct static_lcm {
  static constexpr intmax_t value = (A * B) / static_gcd<A,B>::value;
};

template <class R1, class R2>
struct ratio_gcd {
  // at least the Internet suggests this relationship
  using type = std::ratio<static_gcd<R1::num, R2::num>::value,
                          static_lcm<R1::den, R2::den>::value>;
};

} // namespace detail

template<class Rep, class Period>
class duration;

template<class Clock, class Dur>
class time_point;

template <class T>
struct is_duration : std::false_type {};

template <class Rep, class Period>
struct is_duration<duration<Rep, Period>> : std::true_type {};

} // namespace chrono
} // namespace riot

namespace std {

template <class Rep1, class Period1, class Rep2, class Period2>
struct common_type<riot::chrono::duration<Rep1, Period1>,
                   riot::chrono::duration<Rep2, Period2>> {
  // The period of the resulting duration is the greatest common divisor
  // of Period1 and Period2.
  using type = riot::chrono::duration<
    typename common_type<Rep1, Rep2>::type,
    typename riot::chrono::detail::ratio_gcd<Period1, Period2>::type
  >;
};

template <class Clock, class Duration1, class Duration2>
struct common_type<riot::chrono::time_point<Clock, Duration1>,
                   riot::chrono::time_point<Clock, Duration2>> {
  using type = riot::chrono::time_point<
    Clock,
    typename common_type<Duration1, Duration2>::type
  >;
};

} // namespace std

namespace riot {
namespace chrono {

template <class Rep>
struct duration_values {
public:
  static constexpr Rep zero() {
    return Rep{0};
  };
  static constexpr Rep min() {
    return std::numeric_limits<Rep>::lowest();
  };
  static constexpr Rep max() {
    return std::numeric_limits<Rep>::max();
  }
};

// duration_cast
namespace detail {

template <class ToDur, class CF, class CR,
          bool NumOne = false, bool DenOne = false>
struct duration_cast_impl {
  template <class Rep, class Period>
  static constexpr ToDur cast(const duration<Rep, Period>& d) {
    return ToDur(static_cast<typename ToDur::rep>(
      static_cast<CR>(d.count()) *
      static_cast<CR>(CF::num)   /
      static_cast<CR>(CF::den))
    );
  }
};

template <class ToDur, class CF, class CR>
struct duration_cast_impl<ToDur, CF, CR, true, true> {
  template <class Rep, class Period>
  static constexpr ToDur cast(const duration<Rep, Period>& d) {
    return ToDur(static_cast<typename ToDur::rep>(d.count()));
  }
};

template <class ToDur, class CF, class CR>
struct duration_cast_impl<ToDur, CF, CR, true, false> {
  template <class Rep, class Period>
  static constexpr ToDur cast(const duration<Rep, Period>& d) {
    return ToDur(
      static_cast<typename ToDur::rep>(
        static_cast<CR>(d.count()) / static_cast<CR>(CF::den)
      )
    );
  }
};

template <class ToDur, class CF, class CR>
struct duration_cast_impl<ToDur, CF, CR, false, true> {
  template <class Rep, class Period>
  static constexpr ToDur cast(const duration<Rep, Period>& d) {
    return ToDur(
      static_cast<typename ToDur::rep>(
        static_cast<CR>(d.count()) * static_cast<CR>(CF::num)
      )
    );
  }
};

} // namespace detail

// see: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm#duration
// TODO: check that ToDur is a duration and "Notes" on cppreference
template <class ToDur, class Rep, class Period, typename std::enable_if<
            is_duration<ToDur>::value,
          int>::type = 0>
constexpr ToDur duration_cast(const duration<Rep, Period>& d) {
  using CF
    = typename std::ratio_divide<Period, typename ToDur::period>::type;
  using CR
    = typename std::common_type<typename ToDur::rep, Rep, intmax_t>::type;
  return detail::duration_cast_impl<ToDur, CF, CR, 
                                    CF::num == 1,
                                    CF::den == 1>::cast(d);
}

template <class Rep>
struct treat_as_floating_point : std::is_floating_point<Rep> {};

template <class Rep, class Period = std::ratio<1>>
class duration {
  static_assert(Period::num > 0, "TODO: message");

public:
  using rep = Rep;       // an arithmetic type representing the number of ticks
  using period = Period; // a std::ratio for the tick period (seconds per tick)

public:
  constexpr duration() = default;

  template <class Rep2,
            typename std::enable_if<
              std::is_convertible<rep, Rep2>::value &&
              (treat_as_floating_point<rep>::value ||
               (!treat_as_floating_point<rep>::value &&
                !treat_as_floating_point<Rep2>::value)),
            int>::type = 0>
  constexpr explicit duration(const Rep2& r) : ticks_{static_cast<Rep>(r)} {
    // nop
  }

  template <class Rep2, class Period2,
            typename std::enable_if<
              treat_as_floating_point<rep>::value ||
              (std::ratio_divide<Period2, period>::type::den == 1),
            int>::type = 0>
  constexpr duration(const duration<Rep2, Period2>& d)
    : ticks_{riot::chrono::duration_cast<duration>(d).count()} {
    // nop
  }

  ~duration() = default;

  duration(const duration&) = default;

  duration& operator=(const duration&) = default;

  constexpr rep count() const {
    return ticks_;
  }

  // arithmetic
  constexpr duration operator+() const {
    return duration(*this);
  }

  constexpr duration operator-() const {
    return -duration(*this);
  }

  duration& operator++() {
    ++ticks_;
    return *this;
  }

  duration operator++(int) {
    return duration(ticks_++);
  }

  duration& operator--() {
    --ticks_;
    return *this;
  }

  duration operator--(int) {
    return duration(ticks_--);
  }

  duration& operator+=(const duration& d) {
    ticks_ += d.count();
    return *this;
  }

  duration& operator-=(const duration& d) {
    ticks_ -= d.count();
    return *this;
  }

  duration& operator*=(const rep& rhs) {
    ticks_ *= rhs;
    return *this;
  }

  duration& operator/=(const rep& rhs) {
    ticks_ /= rhs;
    return *this;
  }

  duration& operator%=(const rep& rhs) {
    ticks_ %= rhs;
    return *this;
  }

  duration& operator%=(const duration& d) {
    ticks_ %= d.count();
    return *this;
  }

  static constexpr duration zero() {
    return {duration_values<rep>::zero()};
  }

  static constexpr duration min() {
    return {duration_values<rep>::min()};
  }

  static constexpr duration max() {
    return {duration_values<rep>::max()};
  }

private:
  rep ticks_;
};

// class template time_point
// TODO: might not be right ... but needs a clock to be tested
// see: http://en.cppreference.com/w/cpp/chrono/time_point
template <class Clock, class Duration = typename Clock::duration>
class time_point {
public:
  using clock = Clock;
  using duration = Duration;
  using rep = typename duration::rep;
  using period = typename duration::period;

public:
  constexpr time_point() : d_{0} {
    // nop
  }

  // same as time_point() + d
  constexpr explicit time_point(const duration& d)
    : d_{duration_cast<Duration>(d).count()} {
    // nop
  }

  template <class Duration2>
  constexpr time_point(const time_point<clock, Duration2>& t)
    : d_{duration_cast<duration>(t.time_since_epoch())} {
    // nop
  }

  // observer
  constexpr duration time_since_epoch() const {
    return d_;
  }

  // arithmetic
  time_point& operator+=(const duration& d) {
    d_ += d;
    return d_;
  }

  time_point& operator-=(const duration& d) {
    d_ -= d;
    return d_;
  }

  // TODO: are these the expected values?
  static constexpr time_point min() {
    return {duration::min()};
  }

  static constexpr time_point max() {
    return {duration::max()};
  }

private:
  duration d_;
};

// duration arithmetic
// see: http://en.cppreference.com/w/cpp/chrono/duration/operator_arith4
// lhs	-	duration on the left-hand side of the operator
// rhs	-	duration on the right-hand side of the operator
// d	-	the duration argument for mixed-argument operators
// s	-	tick count argument for mixed-argument operators
// Assuming that CD is the function return type and
//   CR<A, B> = std::common_type<A, B>::type,
// then
//   1) CD(CD(lhs).count() + CD(rhs).count())
//   2) CD(CD(lhs).count() - CD(rhs).count())
// 3-4) CD(CD(d).count() * s)
//   5) CD(CD(d).count() / s).
//   6) CD(lhs).count() / CD(rhs).count() (the return type of this operator is not a duration)
//   7) CD(CD(d).count() % s)
//   8) CD(CD(lhs).count() % CD(rhs).count())
// TODO: some template magic to prevent operations on incompatible durations?
template <class Rep1, class Period1, class Rep2, class Period2>
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
constexpr operator+(const duration<Rep1, Period1>& lhs,
                    const duration<Rep2, Period2>& rhs) {
  using CD = typename std::common_type<duration<Rep1, Period1>,
                                       duration<Rep2, Period2>>::type;
  return CD(CD(lhs).count() + CD(rhs).count());
}

template <class Rep1, class Period1, class Rep2, class Period2>
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
constexpr operator-(const duration<Rep1, Period1>& lhs,
                    const duration<Rep2, Period2>& rhs) {
  using CD = typename std::common_type<duration<Rep1, Period1>,
                                       duration<Rep2, Period2>>::type;
  return CD(CD(lhs).count() - CD(rhs).count());
}

template <class Rep1, class Period, class Rep2>
duration<typename std::common_type<Rep1, Rep2>::type, Period>
constexpr operator*(const duration<Rep1, Period>& d,
                    const Rep2& s) {
  using CD = duration<typename std::common_type<Rep1, Rep2>::type, Period>;
  return CD(CD(d).count() * s);
}

template <class Rep1, class Rep2, class Period>
duration<typename std::common_type<Rep1, Rep2>::type, Period>
constexpr operator*(const Rep1& s,
                    const duration<Rep2, Period>& d) {
  using CD = duration<typename std::common_type<Rep1, Rep2>::type, Period>;
  return CD(CD(d).count() * s);
}

template <class Rep1, class Period, class Rep2>
duration<typename std::common_type<Rep1, Rep2>::type, Period>
constexpr operator/(const duration<Rep1, Period>& d,
                    const Rep2& s) {
  using CD = duration<typename std::common_type<Rep1, Rep2>::type, Period>;
  return CD(CD(d).count() / s);
}

template <class Rep1, class Period1, class Rep2, class Period2>
typename std::common_type<Rep1, Rep2>::type
constexpr operator/(const duration<Rep1, Period1>& lhs,
                    const duration<Rep2, Period2>& rhs) {
  using CD = typename std::common_type<Rep1, Rep2>::type;
  return CD(lhs).count() / CD(rhs).count();
}

template <class Rep1, class Period, class Rep2>
duration<typename std::common_type<Rep1, Rep2>::type, Period>
constexpr operator%(const duration<Rep1, Period>& d,
                    const Rep2& s) {
  using CD = duration<typename std::common_type<Rep1, Rep2>::type, Period>;
  return CD(CD(d).count() % s);
}

template <class Rep1, class Period1, class Rep2, class Period2>
typename std::common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
constexpr operator%(const duration<Rep1, Period1>& lhs,
                    const duration<Rep2, Period2>& rhs) {
  using CD = typename std::common_type<duration<Rep1, Period1>,
                                       duration<Rep2, Period2>>::type;
  return CD(CD(lhs).count() % CD(rhs).count());
}

// duration comparisons
// see: http://en.cppreference.com/w/cpp/chrono/duration/operator_cmp
// lhs	-	duration on the left-hand side of the operator
// rhs	-	duration on the right-hand side of the operator
// Assuming that CT =
// std::common_type<std::chrono::duration<Rep1, Period1>,
//                 std::chrono::duration<Rep2, Period2>>::type, then:
// 1) CT(lhs).count() == CT(rhs).count()
// 2) !(lhs == rhs)
// 3) CT(lhs).count() < CT(rhs).count()
// 4) !(rhs < lhs)
// 5) rhs < lhs
// 6) !(lhs < rhs)
template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator==(const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  using CT = typename std::common_type<duration<Rep1, Period1>,
                                       duration<Rep2, Period2>>::type;
  return CT(lhs).count() == CT(rhs).count();
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator!=(const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  return !(lhs == rhs);
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator< (const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  using CT = typename std::common_type<duration<Rep1, Period1>,
                                       duration<Rep2, Period2>>::type;
  return CT(lhs).count() < CT(rhs).count();
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator<=(const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  return !(rhs < lhs);
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator> (const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  return rhs < lhs;
}

template <class Rep1, class Period1, class Rep2, class Period2>
constexpr bool operator>=(const duration<Rep1, Period1>& lhs,
                          const duration<Rep2, Period2>& rhs) {
  return !(lhs < rhs);
}

using nanoseconds  = duration<int64_t, std::nano>;
using microseconds = duration<int64_t, std::micro>;
using milliseconds = duration<int64_t, std::milli>;
using seconds      = duration<int64_t>;
using minutes      = duration<int32_t, std::ratio< 60>>;
using hours        = duration<int32_t, std::ratio<3600>>;

template <class Clock, class Duration1, class Rep2, class Period2>
constexpr time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>
operator+(const time_point<Clock, Duration1>& lhs,
          const duration<Rep2, Period2>& rhs) {
  using CT 
    = typename std::common_type<Duration1, duration<Rep2, Period2>>::type;
  return time_point<Clock, CT>{duration_cast<CT>(lhs.time_since_epoch())} + duration_cast<CT>(rhs);
}
template <class Rep1, class Period1, class Clock, class Duration2>
constexpr time_point<Clock, typename std::common_type<duration<Rep1, Period1>, Duration2>::type>
operator+(const duration<Rep1, Period1>& lhs,
          const time_point<Clock, Duration2>& rhs) {
  return rhs +lhs;
}

template <class Clock, class Duration1, class Rep2, class Period2>
constexpr time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>
operator-(const time_point<Clock, Duration1>& lhs,
          const duration<Rep2, Period2>& rhs) {
  using CT
    = typename std::common_type<Duration1, duration<Rep2, Period2>>::type;
  return time_point<Clock, CT>{duration_cast<CT>(lhs.time_since_epoch())} - duration_cast<CT>(rhs);
}

template <class Clock, class Duration1, class Duration2>
constexpr typename std::common_type<Duration1, Duration2>::type
operator-(const time_point<Clock, Duration1>& lhs,
          const time_point<Clock, Duration2>& rhs) {
  using CT = typename std::common_type<Duration1, Duration2>::type;
  return duration_cast<CT>(lhs) - duration_cast<CT>(rhs);
}


// time_point comparisons
template <class Clock, class Duration1, class Duration2>
constexpr bool operator==(const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  using CT = typename std::common_type<Duration1, Duration2>::type;
  return duration_cast<CT>(lhs.time_since_epoch()) ==
    duration_cast<CT>(rhs.time_since_epoch());
}


template <class Clock, class Duration1, class Duration2>
constexpr bool operator!=(const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  return !(lhs == rhs);
}

template <class Clock, class Duration1, class Duration2>
constexpr bool operator< (const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  using CT = typename std::common_type<Duration1, Duration2>::type;
  return duration_cast<CT>(lhs.time_since_epoch()) <
    duration_cast<CT>(rhs.time_since_epoch());
}


template <class Clock, class Duration1, class Duration2>
constexpr bool operator<=(const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  return !(rhs > lhs);
}

template <class Clock, class Duration1, class Duration2>
constexpr bool operator> (const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  return rhs < lhs;
}

template <class Clock, class Duration1, class Duration2>
constexpr bool operator>=(const time_point<Clock, Duration1>& lhs,
                          const time_point<Clock, Duration2>& rhs) {
  return !(lhs < rhs);
}

// time_point_cast
template <class ToDur, class Clock, class Duration,
          typename std::enable_if<
            is_duration<ToDur>::value,
          int>::type = 0>
constexpr time_point<Clock, ToDur>
time_point_cast(const time_point<Clock, Duration>& t) {
  return time_point<Clock, ToDur>{duration_cast<ToDur>(t.time_since_epoch())};
}


// clocks
//class system_clock;
//class steady_clock;
//class high_resolution_clock;

class system_clock {
public:
  using duration = microseconds;
  using rep = duration::rep;
  using period = duration::period;
  using time_point = chrono::time_point<system_clock, duration>;

  static constexpr bool is_steady = false;

  static time_point now() noexcept;

  static timex_t to_time_t (const time_point& t) noexcept;

  static time_point from_time_t(timex_t t) noexcept;

private:
  static constexpr rep microsecs_in_sec = 1000000;
};

/*
class high_resolution_clock {
public:
  using rep = //unspecified ;
  using period = ratio<//unspecified, unspecified>;
  using duration = chrono::duration<rep, period>;
  using time_point = chrono::time_point<//unspecified, duration>;
  static constexpr bool is_steady = //unspecified ;
  static time_point now() noexcept;
};

*/

} // namespace chrono
} // namespace riot

/// ########## old chrono impl ##########

#include <chrono>

namespace riot {

namespace {
constexpr uint32_t microsecs_in_sec = 1000000;
} // namespace anaonymous

/**
 * @brief A time point for timed wait, as clocks from the standard are not
 *        available on RIOT.
 */
class time_point {
  using native_handle_type = timex_t;

 public:
  /**
   * @brief Creates a time point with seconds and microseconds set to 0.
   */
  inline time_point() : m_handle{0, 0} {}
  /**
   * @brief Create time point from timex_t struct.
   */
  explicit inline time_point(timex_t&& tp) : m_handle(tp) {}
  /**
   * @brief Use default copy constructor.
   */
  constexpr time_point(const time_point& tp) = default;
  /**
   * @brief Use default move constructor.
   */
  constexpr time_point(time_point&& tp) = default;

  /**
   * @brief Gives access to the native handle that stores the time information.
   */
  inline native_handle_type native_handle() const { return m_handle; }

  /**
   * @brief Add a standard chrono::duration to this time point.
   */
  template <class Rep, class Period>
  inline time_point& operator+=(const std::chrono::duration<Rep, Period>& d) {
    auto s = std::chrono::duration_cast<std::chrono::seconds>(d);
    auto m = (std::chrono::duration_cast<std::chrono::microseconds>(d) - s);
    m_handle.seconds += s.count();
    m_handle.microseconds += m.count();
    adjust_overhead();
    return *this;
  }

  /**
   * @brief Returns seconds member as uint32_t.
   */
  inline uint32_t seconds() const { return m_handle.seconds; }

  /**
   * @brief Returns microseconds member as uint32_t.
   */
  inline uint32_t microseconds() const { return m_handle.microseconds; }

 private:
  timex_t m_handle;
  void inline adjust_overhead() {
    auto secs = m_handle.microseconds / microsecs_in_sec;
    m_handle.seconds += secs;
    m_handle.microseconds -= (secs * microsecs_in_sec);
  }
};

/**
 * @brief Returns the current time saved in a time point.
 *
 * @return time_point containing the current time.
 */
inline time_point now() {
  timex_t tp;
  xtimer_now_timex(&tp);
  return time_point(std::move(tp));
}

/**
 * @brief Compares two timepoints.
 */
inline bool operator<(const time_point& lhs, const time_point& rhs) {
  return lhs.seconds() < rhs.seconds()
         || (lhs.seconds() == rhs.seconds() && lhs.microseconds()
                                               < rhs.microseconds());
}

/**
 * @brief Compares two timepoints.
 */
inline bool operator>(const time_point& lhs, const time_point& rhs) {
  return rhs < lhs;
}

/**
 * @brief Compares two timepoints.
 */
inline bool operator<=(const time_point& lhs, const time_point& rhs) {
  return !(rhs < lhs);
}

/**
 * @brief Compare two timepoints.
 */
inline bool operator>=(const time_point& lhs, const time_point& rhs) {
  return !(lhs < rhs);
}

} // namespace riot

#endif // RIOT_CHRONO_HPP
