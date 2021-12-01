#ifndef PRIMES_H
#define PRIMES_H

#include <algorithm>
#include <atomic>
#include <deque>

namespace primes {
class SpinLock {
  private:
   std::atomic_flag flag = ATOMIC_FLAG_INIT;
   public:
   void acquire() {
     while (flag.test_and_set(std::memory_order_acquire))
       ;
   }
   void release() { flag.clear(std::memory_order_release); }
};

class PrimeOutputBuffer {
    private:
     SpinLock lock;
     std::deque<unsigned long long> theQueue;
    public:
     void push(unsigned long long value) {
       lock.acquire();
       theQueue.push_back(value);
       lock.release();
       }
       unsigned long long pop() {
         lock.acquire();
         unsigned long long result = theQueue.front();
         theQueue.pop_front();
         lock.release();
         return result;
       }
       int size() {
         lock.acquire();
         int result = theQueue.size();
         lock.release();
         return result;
          }
          unsigned long long peekBack() {
            lock.acquire();
            unsigned long long result = theQueue.back();
            lock.release();
            return result;
           }
           unsigned long long get(size_t index) {
             lock.acquire();
             unsigned long long result = theQueue.at(index);
             lock.release();
             return result;
           }
           void sort () {
             lock.acquire();
             std::sort(theQueue.begin(), theQueue.end());
             lock.release();
           }
           unsigned long long operator[](size_t index) {
             lock.acquire();
             unsigned long long result = theQueue[index];
             lock.release();
             return result;
            }
};
void getPrimes(int numPrimes, PrimeOutputBuffer& outputBuffer);
}

#endif