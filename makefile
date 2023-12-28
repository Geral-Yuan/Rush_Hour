CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
FLTKFLAGS = -lfltk

rush:
	$(CXX) ./src/main.cpp -o main $(CXXFLAGS) $(FLTKFLAGS)

debug:
	$(CXX) ./src/main.cpp -g -o main $(CXXFLAGS) $(FLTKFLAGS)

clean:
	rm main