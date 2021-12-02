#include "primes.h"
#include <iostream>

static const int numPrimes = 1000000;

using namespace primes;
using namespace std;

static void computeDeltas(PrimeOutputBuffer& input, PrimeOutputBuffer& output) {
  unsigned long long previous = 0;
  for (int i = 0; i < input.size(); i ++) {
    unsigned long long delta = input[i] - previous;
    output.push(delta);
    previous = input[i];
  }
  }

  double computeMean(PrimeOutputBuffer& input) {
    unsigned long long counter = 0;
    for (int i = 0; i < input.size(); i ++) {
      counter += input[i];
    }
    return (double)counter / (double)input.size();
    }

double computeMedian(PrimeOutputBuffer& input) {
  if (input.size() %2 == 1) {
    return (double)input[input.size() / 2 + 1];
  }else {
    return ((double)input[input.size() / 2] +
            (double)input[input.size() / 2 + 1]) /
           2;
  }
}

    int main() {
        PrimeOutputBuffer primes;
        getPrimes(numPrimes, primes);
        PrimeOutputBuffer deltas;
        computeDeltas(primes, deltas);
        double meanDelta = computeMean(deltas);
        cout << "Mean delta: " << meanDelta << endl;
        deltas.sort();
        double medianDelta = computeMedian(deltas);
        cout << "Median delta: " << medianDelta << endl;
        return 0;
    }
