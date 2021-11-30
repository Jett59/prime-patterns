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
           unsigned long long result = theQueue.front();
           theQueue.pop();
           lock.unlock();
           return result;
       }
       int size() { return theQueue.size(); }
       unsigned long long peekBack() { return theQueue.back(); }
};

struct WorkerContext {
    int start;
  int increment;
  int foundPrimes = 0;
  primeOutputBuffer* outputBuffer;
  bool running = false;
  bool shouldBeRunning = true;
};

int primeFinderWorker(volatile WorkerContext* context) {
  context->running = true;
  for (unsigned long long i = context->start; context->running; i += context->increment) {
    if (isPrime(i)) {
      context->outputBuffer->push(i);
      context->foundPrimes++;
    }
    if (!context->shouldBeRunning) {
      this_thread::sleep_for(chrono::milliseconds(100));
      context->running = false;
    }
}
return 0;
}

int main (int argc, char** argv) {
  auto numWorkers = thread::hardware_concurrency();
  volatile WorkerContext contexts[numWorkers];
  primeOutputBuffer outputBuffer;
  cout << "Starting calculation..." << endl;
  for (int i = 0; i < numWorkers; i ++) {
    volatile WorkerContext& context = contexts[i];
    context.start = i + 2;
    context.increment = numWorkers;
    context.outputBuffer = &outputBuffer;
    thread workerThread(primeFinderWorker, &contexts[i]);
    workerThread.detach();
  }
  int targetNumberOfPrimes = atoi(argv[1]);
  while (outputBuffer.size() < targetNumberOfPrimes) {
    cout << outputBuffer.size() << ": " << outputBuffer.peekBack() << "\t\r";
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  cout << "Stopping worker threads" << endl;
  for (int i = 0; i < numWorkers; i ++) {
    contexts[i].shouldBeRunning = false;
    cout << "Stopping worker thread " << i << ": ";
    while (contexts[i].running)
    ;
    cout << "Done" << endl;
  }
    cout << "Found " << outputBuffer.size() << " primes" << endl;
    cout << "Last prime: " << outputBuffer.peekBack() << endl;
    return 0;
}
