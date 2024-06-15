CXX = clang++
CXXFLAGS = -std=c++20 -fcolor-diagnostics -fansi-escape-codes -Wall -Wextra -g -O3

build/day01: src/day01.cpp src/helper.hpp
	$(CXX) $(CXXFLAGS) src/day01.cpp -o build/day01
	build/day01


