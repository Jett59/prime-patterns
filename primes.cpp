#include <iostream>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename IntType>
constexpr bool isDivisible(IntType dividend, IntType divisor) {
    return dividend % divisor == 0;
}
template<typename IntType>
constexpr bool internalNotIsPrime(IntType n, IntType testNumber) {
    return isDivisible<IntType>(n, testNumber) ? true : (testNumber <= n / 2) ? internalNotIsPrime(n, testNumber + 1) : false;
}
template<typename IntType>
constexpr bool isPrime(IntType n) {
    return n == 2 ? true : !internalNotIsPrime<IntType>(n, 2);
}

using namespace std;

class primeOutputBuffer {
    private:
     mutex theMutex;
     queue<unsigned long long> theQueue;
     public:
      void push(unsigned long long value) {
          unique_lock<mutex> lock(theMutex);
          theQueue.push(value);
          lock.unlock();
       }
       unsigned long long pop() {
           unique_lock<mutex> lock(theMutex);
           unsigned long long result = theQueue.back();
           theQueue.pop();
           lock.unlock();
           return result;
       }
       int size() { return theQueue.size(); }
};

struct WorkerContext {
    int start;
  int increment;
  int foundPrimes = 0;
  primeOutputBuffer* outputBuffer;
};

int primeFinderWorker(WorkerContext* context) {
    for (unsigned long long i = context->start;; i += context->increment) {
if (isPrime(i)) {
  context->outputBuffer->push(i);
  context->foundPrimes++;
}
}
}

int main (int argc, char** argv) {
  auto numWorkers = thread::hardware_concurrency();
  WorkerContext contexts[numWorkers];
  primeOutputBuffer outputBuffer;
  for (int i = 0; i < numWorkers; i ++) {
    WorkerContext context;
    context.start = i + 2;
    context.increment = numWorkers;
    context.outputBuffer = &outputBuffer;
    contexts[i] = context;
    thread workerThread(primeFinderWorker, &contexts[i]);
    workerThread.detach();
  }
  while (true) {
      while (outputBuffer.size() < 1)
        ;
      cout << outputBuffer.pop() << " with " << outputBuffer.size() << "Remaining" << endl;
  }
  return 0;
}
