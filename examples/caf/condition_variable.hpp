
#ifndef CONDITION_VARIABLE_HPP
#define CONDITION_VARIABLE_HPP

#include <chrono>

#include "mutex.hpp"

namespace caf {

enum class cv_status { no_timeout, timeout };

class condition_variable {
  
 public:
  using native_handle_type = priority_queue_t*;

  // constexpr condition_variable() : m_queue{NULL} { }
  inline condition_variable() {
    m_queue.first = NULL;
  }
  ~condition_variable();

  void notify_one() noexcept;
  void notify_all() noexcept;

  void wait(unique_lock<mutex>& lock) noexcept;
  template <class Predicate>
  void wait(unique_lock<mutex>& lock, Predicate pred);
  template <class Clock, class Duration>
  cv_status wait_until(unique_lock<mutex>& lock,
                 const std::chrono::time_point<Clock, Duration>& timeout_time);
  template <class Clock, class Duration, class Predicate>
  bool wait_until(unique_lock<mutex>& lock,
                  const std::chrono::time_point<Clock,Duration>& timeout_time,
                  Predicate pred);
  // template <class Rep, class Period>
  // cv_status wait_for(unique_lock<mutex>& lock,
  //                    const std::chrono::duration<Rep, Period>& rel_time);
  // template <class Rep, class Period, class Predicate>
  // bool wait_for(unique_lock<mutex>& lock,
  //               const std::chrono::duration<Rep, Period>& rel_time,
  //               Predicate pred);
  inline native_handle_type native_handle() { return &m_queue; }

 private:
  condition_variable(const condition_variable&);
  condition_variable& operator=(const condition_variable&);

  void do_timed_wait(unique_lock<mutex>& lock,
                     std::chrono::time_point<std::chrono::system_clock, 
                                             std::chrono::nanoseconds>) noexcept;

  priority_queue_t m_queue;
};

template <class T, class Rep, class Period>
inline typename std::enable_if<std::chrono::__is_duration<T>::value,T>::type
ceil(std::chrono::duration<Rep, Period> duration) {
  using namespace std::chrono;
  T res = duration_cast<T>(duration);
  if (res < duration) {
    ++res;
  }
  return res;
}

template <class Predicate>
void condition_variable::wait(unique_lock<mutex>& lock, Predicate pred) {
  while(!pred()) {
    wait(lock);
  }
}

template <class Clock, class Duration>
cv_status condition_variable::wait_until(unique_lock<mutex>& lock,
                                         const std::chrono::time_point<Clock,
                                                      Duration>& timeout_time) {
    using namespace std::chrono;
    wait_for(lock, timeout_time - system_clock::now());
    return std::chrono::system_clock::now() < timeout_time ?
      cv_status::no_timeout : cv_status::timeout;
}

template <class Clock, class Duration, class Predicate>
bool condition_variable::wait_until(unique_lock<mutex>& lock,
                    const std::chrono::time_point<Clock,Duration>& timeout_time,
                    Predicate pred) {
  while (!pred()) {
    if (wait_until(lock, timeout_time) == cv_status::timeout) {
      return pred();
    }
  }
  return true;
}

} // namespace caf

#endif // CONDITION_VARIABLE_HPP