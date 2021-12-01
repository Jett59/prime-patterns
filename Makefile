CXXFLAGS+=-O3 -pthread
CXXFLAGS+=-fno-math-errno

a.out: primes.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) primes.cpp -o a.out

clean:
	rm a.out
