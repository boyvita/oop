#pragma once
#include <fstream>

class iThreadable {
public:
	virtual void read(std::ifstream &) = 0;
	virtual void calculate() = 0;
	virtual void print_answer(std::ofstream &) = 0;
};