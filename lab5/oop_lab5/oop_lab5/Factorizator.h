#pragma once
#include "iThreadable.h"
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <cmath>
#include <stdint.h>
#include <fstream>

class Factorizator :  public iThreadable {
public:
	Factorizator();
	~Factorizator();
	Factorizator(uint64_t const& number);
	uint64_t get_number();
	void read(std::ifstream & in);
	void calculate();
	std::string get_answer();
	void print_answer(std::ofstream & out);
private:
	uint64_t number;
	std::vector<std::pair<uint32_t, uint8_t>> factors;
	std::string ans;
};

