#include <iostream>
#include <atomic>
#include <chrono>

#include "thread.hpp"
#include "condition_variable.hpp"

using namespace std;
using namespace caf;
 
condition_variable cv;
mutex cv_m;
atomic<int> i = ATOMIC_VAR_INIT(0);
atomic<int> j = ATOMIC_VAR_INIT(0);
 
void waits(int idx)
{
    unique_lock<mutex> lk(cv_m);
    auto now = chrono::system_clock::now();
    if(cv.wait_until(lk, now + chrono::milliseconds(idx*100), [](){return i == 1;}))
    {
        cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
    }
    else
    {
        cerr << "Thread " << idx << " timed out. i == " << i << '\n';
        ++j;
    }
}
 
void signals()
{
    this_thread::sleep_for(chrono::milliseconds(120));
    cerr << "Notifying...\n";
    cv.notify_all();
    this_thread::sleep_for(chrono::milliseconds(100));
    i = 1;
    cerr << "Notifying again...\n";
    cv.notify_all();
}
 
int main()
{
    thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    /*
    t1.join(); 
    t2.join();
    t3.join();
    t4.join();
    */
    unique_lock<mutex> lk(cv_m);
    cv.wait(lk, [] { return j==4; });
}

