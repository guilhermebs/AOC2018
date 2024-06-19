CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -O3

build/day01: src/day01.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day01.cpp -o build/day01
	build/day01


build/day02: src/day02.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day02.cpp -o build/day02
	build/day02

build/day03: src/day03.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day03.cpp -o build/day03
	build/day03

build/day04: src/day04.cpp
	$(CXX) $(CXXFLAGS) src/day04.cpp -o build/day04
	build/day04

build/day05: src/day05.cpp
	$(CXX) $(CXXFLAGS) src/day05.cpp -o build/day05
	build/day05

build/day06: src/day06.cpp
	$(CXX) $(CXXFLAGS) src/day06.cpp -o build/day06
	build/day06

build/day07: src/day07.cpp
	$(CXX) $(CXXFLAGS) src/day07.cpp -o build/day07
	build/day07




