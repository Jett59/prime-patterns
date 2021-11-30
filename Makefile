CXXFLAGS+=-O3 -pthread

a.out: primes.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) primes.cpp -o a.out

clean:
	rm a.out
