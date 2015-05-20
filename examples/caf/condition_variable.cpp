
#include <system_error>

extern "C" {
#include "irq.h"
#include "sched.h"
}

#include "condition_variable.hpp"

using namespace std::chrono;

namespace caf {

condition_variable::~condition_variable() {
  m_queue.first = NULL;   
}

void condition_variable::notify_one() noexcept {
  unsigned old_state = disableIRQ();
  priority_queue_node_t *head = priority_queue_remove_head(&m_queue);
  int other_prio = -1;
  if (head != NULL) {
    tcb_t *other_thread = (tcb_t *) sched_threads[head->data];
    if (other_thread) {
      other_prio = other_thread->priority;
      sched_set_status(other_thread, STATUS_PENDING);
    }
    head->data = -1u;
  }
  restoreIRQ(old_state);
  if (other_prio >= 0) {
    sched_switch(other_prio);
  }
}

void condition_variable::notify_all() noexcept {
  unsigned old_state = disableIRQ();
  int other_prio = -1;
  while (true) {
    priority_queue_node_t *head = priority_queue_remove_head(&m_queue);
    if (head == NULL) {
      break;
    }
    tcb_t *other_thread = (tcb_t *) sched_threads[head->data];
    if (other_thread) {
      auto max_prio = [](int a, int b) {
        return (a < 0) ? b : ((a < b) ? a : b);
      };
      other_prio = max_prio(other_prio, other_thread->priority);
      sched_set_status(other_thread, STATUS_PENDING);
    }
    head->data = -1u;
  }
  restoreIRQ(old_state);
  if (other_prio >= 0) {
    sched_switch(other_prio);
  }
}

void condition_variable::wait(unique_lock<mutex>& lock) noexcept {
  if (!lock.owns_lock()) {
    std::__throw_system_error(EPERM,
                              "condition_variable::wait: mutex not locked");
  }
  priority_queue_node_t n;
  n.priority = sched_active_thread->priority;
  n.data = sched_active_pid;
  n.next = NULL;

  /* the signaling thread may not hold the mutex, the queue is not thread safe */
  unsigned old_state = disableIRQ();
  priority_queue_add(&m_queue, &n);
  restoreIRQ(old_state);
  mutex_unlock_and_sleep(lock.mutex()->native_handle());
  if (n.data != -1u) {
    /* on signaling n.data is set to -1u */
    /* if it isn't set, then the wakeup is either spurious or a timer wakeup */
    old_state = disableIRQ();
    priority_queue_remove(&m_queue, &n);
    restoreIRQ(old_state);
  }
  mutex_lock(lock.mutex()->native_handle());
}

// void condition_variable::__do_timed_wait(unique_lock<mutex>& lock, 
//                                          time_point<system_clock,
//                                                     nanoseconds> tp) noexcept {
//   if (!lock.owns_lock()) {
//     std::__throw_system_error(EPERM,
//                               "condition_variable::timed wait: mutex not locked");
//   }
//   nanoseconds d = tp.time_since_epoch();
//   if (d > nanoseconds(0x59682F000000E941)) {
//     d = nanoseconds(0x59682F000000E941);
//   }
//   timespec ts;
//   seconds s = duration_cast<seconds>(d);
//   typedef decltype(ts.tv_sec) ts_sec;
//   constexpr ts_sec ts_sec_max = numeric_limits<ts_sec>::max();
//   if (s.count() < ts_sec_max) {
//     ts.tv_sec = static_cast<ts_sec>(s.count());
//     ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>((d - s).count());
//   } else {
//     ts.tv_sec = ts_sec_max;
//     ts.tv_nsec = giga::num - 1;
//   }
//   // auto rc = pthread_cond_timedwait(&cond, lock.mutex()->native_handle(), &ts);
//   // timex_t now, then, reltime;
//   // vtimer_now(&now);
//   // then.seconds = abstime->tv_sec;
//   // then.microseconds = abstime->tv_nsec / 1000u;
//   // reltime = timex_sub(then, now);
//   // vtimer_t timer;
//   // vtimer_set_wakeup(&timer, reltime, sched_active_pid);
//   // int result = pthread_cond_wait(cond, mutex);
//   // vtimer_remove(&timer);
//   auto rc = result; // delete this line
//   if (rc != 0 && rc != ETIMEDOUT) {
//     __throw_system_error(ec, "condition_variable timed_wait failed");
//   }
// }

} // namespace caf
