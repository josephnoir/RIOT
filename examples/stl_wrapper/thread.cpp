
#include <time.h>

#include <cerrno>
#include <system_error>

#include "caf/thread.hpp"

using namespace std;

namespace caf {

thread::~thread() {
  if (joinable()) {
    terminate();
  }
}

void thread::join() {
  if (this->get_id() == this_thread::get_id()) {
    throw system_error(make_error_code(errc::resource_deadlock_would_occur),
                            "Joining this leads to a deadlock.");
  }
  if (joinable()) {
    m_handle = thread_uninitialized;
    auto status = thread_getstatus(m_handle);
    if (status != STATUS_NOT_FOUND && status != STATUS_STOPPED) {
      m_data->joining_thread = sched_active_pid;
      thread_sleep();
    }
  } else {
    throw system_error(make_error_code(errc::invalid_argument),
                            "Can not join an unjoinable thread.");
  }
  // missing: no_such_process system error
}

void thread::detach() {
  if (joinable()) {
    m_handle = thread_uninitialized;
  } else {
    throw system_error(make_error_code(errc::invalid_argument),
                            "Can not detach an unjoinable thread.");
  }
}

unsigned thread::hardware_concurrency() noexcept {
  return 1;
}

namespace this_thread {

void sleep_for(const chrono::nanoseconds& ns) {
  using namespace chrono;
  if (ns > nanoseconds::zero()) {
    seconds s = duration_cast<seconds>(ns);
    timespec ts;
    using ts_sec = decltype(ts.tv_sec);
    // typedef decltype(ts.tv_sec) ts_sec;
    constexpr ts_sec ts_sec_max = numeric_limits<ts_sec>::max();
    if (s.count() < ts_sec_max) {
        ts.tv_sec = static_cast<ts_sec>(s.count());
        ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>((ns-s).count());
    } else {
      ts.tv_sec = ts_sec_max;
      ts.tv_nsec = giga::num - 1;
    }
    while (nanosleep(&ts, &ts) == -1 && errno == EINTR) { ; }
  }
}

} // namespace this_thread

} // namespace caf
