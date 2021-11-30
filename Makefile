a.out: primes.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -O3 primes.cpp -o a.out

clean:

	rm a.out
