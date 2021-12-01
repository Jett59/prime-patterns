CXXFLAGS+=-O3 -pthread
CXXFLAGS+=-fno-math-errno

a.out: primes.cpp patterns.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm a.out
