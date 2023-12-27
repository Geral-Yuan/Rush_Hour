CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

rush:
	$(CXX) ./src/main.cpp -o main $(CXXFLAGS)

debug:
	$(CXX) ./src/main.cpp -g -o main $(CXXFLAGS)

clean:
	rm main