

#ifndef THREAD_HPP
#define THREAD_HPP

#include <tuple>
#include <chrono>
#include <utility>
#include <exception>
#include <functional>
#include <type_traits>
#include <system_error>

extern "C" {
#include "thread.h"
}

#include "mutex.hpp"
#include "condition_variable.hpp"
#include "thread_util.hpp"

#include "caf/detail/int_list.hpp"
#include "caf/detail/apply_args.hpp"

class thread;
class thread_id;

namespace caf {

namespace {
  constexpr kernel_pid_t thread_uninitialized = -1;
}

class thread_id {
  template<class CharT,class Traits>
  friend std::basic_ostream<CharT,Traits>&
    operator<<(std::basic_ostream<CharT,Traits>& out, thread_id id);
  friend class thread;

 public:
  inline thread_id() noexcept : m_handle{thread_uninitialized} { };
  inline thread_id(kernel_pid_t handle) : m_handle{handle} {}

  inline bool operator==(thread_id other) noexcept {
    return m_handle == other.m_handle;
  }
  inline bool operator!=(thread_id other) noexcept {
    return !(m_handle == other.m_handle);
  }
  inline bool operator< (thread_id other) noexcept {
    return m_handle < other.m_handle;
  }
  inline bool operator<=(thread_id other) noexcept {
    return !(m_handle > other.m_handle);  
  }
  inline bool operator> (thread_id other) noexcept {
    return m_handle > other.m_handle;
  }
  inline bool operator>=(thread_id other) noexcept {
    return !(m_handle < other.m_handle);  
  }

 private:
  kernel_pid_t m_handle;
};

template<class CharT,class Traits>
inline std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& out, thread_id id) {
  return out << id.m_handle;
}

namespace this_thread {

  inline thread_id get_id() noexcept { return thread_getpid(); };
  inline void yield() noexcept { thread_yield(); }; 
  void sleep_for(const std::chrono::nanoseconds& ns);
  template <class Rep, class Period>
  void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration) {
    // using namespace std::chrono;
    // if (sleep_duration > duration<Rep,Period>::zero()) {
    //   constexpr duration<long double> max = nanoseconds::max();
    //   nanoseconds ns;
    //   if (sleep_duration < max) {
    //     ns = duration_cast<nanoseconds>(sleep_duration);
    //     if (ns < sleep_duration) {
    //       ++ns;
    //     }
    //   }
    //   else {
    //     ns = nanoseconds::max();
    //   }
    //   sleep_for(ns);
    // }
  }
  template <class Clock, class Period>
  void sleep_until(const std::chrono::time_point<Clock, Period>& sleep_time) {
    // using namespace std::chrono;
    // std::mutex mtx;
    // std::condition_variable cv;
    // unique_lock<mutex> lk(mtx);
    // while(Clock::now() < sleep_time) {
    //   cv.wait_until(lk,sleep_time);
    // }
  }
  template <class Period>
  inline void
  sleep_until(const std::chrono::time_point<std::chrono::steady_clock,
                                            Period>& sleep_time) {
      using namespace std::chrono;
      sleep_for(sleep_time - steady_clock::now());
  }
} // namespace this_thread

class thread {

 public:
  using id = thread_id;
  using native_handle_type = kernel_pid_t;

  inline thread() noexcept : m_handle{thread_uninitialized} { };
  template <class F, class ...Args,
            class = typename std::enable_if<
              !std::is_same<typename std::decay<F>::type,thread>::value
            >::type
           >
  explicit thread(F&& f, Args&&... args);
  ~thread();

  thread(const thread&) = delete;
  inline thread(thread&& t) noexcept : m_handle{t.m_handle} {
    t.m_handle = thread_uninitialized;
  }
  thread& operator=(const thread&) = delete;
  thread& operator=(thread&&) noexcept;

  void swap(thread& t) noexcept { std::swap(m_handle, t.m_handle); }

  inline bool joinable() const noexcept { return false; }
  void join();
  void detach();
  inline id get_id() const noexcept { return m_handle; }
  inline native_handle_type native_handle() noexcept { return m_handle; }

  static unsigned hardware_concurrency() noexcept;

  kernel_pid_t m_handle;
  char m_stack[KERNEL_CONF_STACKSIZE_MAIN];
};

void swap(thread& lhs, thread& rhs) noexcept;

/*
template<class T>
class thread_storage_ptr { };

class sub_state;
class thread_struct_impl;

class thread_struct {
 public:
  thread_struct();
  ~thread_struct();

  // void notify_add_on_exit(condition_variable*,mutex*);
  // void make_ready_at_thread_exit(sub_state*);

 private:
  thread_struct_impl* impl;

  thread_struct(const thread_struct&);
  thread_struct& operator =(const thread_struct&);
};
*/

template <class F>
void* thread_proxy(void* vp) {
  std::unique_ptr<F> p(static_cast<F*>(vp));
  auto indices = 
    detail::get_right_indices<caf::detail::tl_size<F>::value - 1>(*p);
  detail::apply_args(std::get<0>(*p), indices, *p);
  return nullptr;
}

template <class F, class ...Args,
         class
        >
thread::thread(F&& f, Args&&... args) {
 using namespace std;
 using func_and_args = tuple<typename decay<F>::type,
                             typename decay<Args>::type...>;
 std::unique_ptr<func_and_args> p(new func_and_args(decay_copy(forward<F>(f)),
                                    decay_copy(forward<Args>(args))...));
 m_handle = thread_create(m_stack, sizeof(m_stack), 
                          PRIORITY_MAIN - 1, 0, // CREATE_WOUT_YIELD
                          &thread_proxy<func_and_args>,
                          p.get(), "caf_thread");
 if (m_handle >= 0) {
   p.release();
 } else {
   std::__throw_system_error(static_cast<int>(m_handle),
                             "Failed to create thread.");
 }
}

inline thread& thread::operator=(thread&& other) noexcept {
  if (m_handle != 0) {
    std::terminate();
  }
  m_handle = other.m_handle;
  other.m_handle = 0;
  return *this;
}

inline void swap (thread& lhs, thread& rhs) noexcept {
  lhs.swap(rhs);
}

} // namespace caf

#endif
