#include <iostream>
#include <cstdlib>
#include <cmath>

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

int main (int argc, char** argv) {
for (unsigned long long i = 2147483647ull;; i += 2) {
if (isPrime(i)) {
cout << i << endl;
}
}
return 0;
}
