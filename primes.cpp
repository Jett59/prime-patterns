#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <thread>
#include "primes.h"

namespace primes {
template<typename IntType>
constexpr bool isDivisible(IntType dividend, IntType divisor) {
    return dividend % divisor == 0;
}
template<typename IntType>
constexpr bool internalNotIsPrime(IntType n, IntType testNumber, IntType top) {
  return testNumber <= top ? isDivisible(n, testNumber)
                                 ? true
                                 : internalNotIsPrime(n, testNumber + 1, top) : false;
}
template<typename IntType>
constexpr bool isPrime(IntType n) {
    return n == 2 ? true : !internalNotIsPrime<IntType>(n, 2, static_cast<IntType>(sqrt(static_cast<double>(n))));
}

using namespace std;

struct WorkerContext {
    int start;
  int increment;
  int foundPrimes = 0;
  PrimeOutputBuffer* outputBuffer;
  bool running = false;
  bool shouldBeRunning = true;
};

static int primeFinderWorker(volatile WorkerContext* context) {
  context->running = true;
  for (unsigned long long i = context->start; context->running; i += context->increment) {
    if (isPrime(i)) {
      context->outputBuffer->push(i);
      context->foundPrimes++;
    }
    if (!context->shouldBeRunning) {
      context->running = false;
    }
}
return 0;
}

void getPrimes(int numPrimes, PrimeOutputBuffer& outputBuffer) {
  auto numWorkers = thread::hardware_concurrency();
  volatile WorkerContext contexts[numWorkers];
  cout << "Starting calculation..." << endl;
  for (int i = 0; i < numWorkers; i ++) {
    volatile WorkerContext& context = contexts[i];
    context.start = i * 2 + 3; // Only odd numbers can be prime (except 2)
    context.increment = numWorkers * 2;
    context.outputBuffer = &outputBuffer;
    thread workerThread(primeFinderWorker, &contexts[i]);
    workerThread.detach();
  }
  while (outputBuffer.size() < numPrimes) {
    cout << outputBuffer.size() << ": " << outputBuffer.peekBack() << "\t\r";
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  cout << "Stopping worker threads" << endl;
  for (int i = 0; i < numWorkers; i ++) {
    contexts[i].shouldBeRunning = false;
    cout << "Stopping worker thread " << i << ": ";
    while (contexts[i].running)
    ;
    cout << "Done (it found " << contexts[i].foundPrimes << " primes)" << endl;
  }
    cout << "Found " << outputBuffer.size() << " primes" << endl;
    cout << "Sorting..." << endl;
    outputBuffer.sort();
    cout << "Last prime: " << outputBuffer.peekBack() << endl;    
}
}
