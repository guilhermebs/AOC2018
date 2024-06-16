CXX = clang++
CXXFLAGS = -std=c++20 -fcolor-diagnostics -fansi-escape-codes -Wall -Wextra -g -O3

build/day01: src/day01.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day01.cpp -o build/day01
	build/day01


build/day02: src/day02.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day02.cpp -o build/day02
	build/day02

build/day03: src/day03.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day03.cpp -o build/day03
	build/day03




