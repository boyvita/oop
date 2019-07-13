#pragma once
#include "Factorizator.h"
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <cmath>
#include <stdint.h>
#include <fstream>

Factorizator::Factorizator() = default;

Factorizator::~Factorizator() = default;

Factorizator::Factorizator(uint64_t const& number) : number(number) {}

uint64_t Factorizator::get_number() {
	return number;
}

void Factorizator::read(std::ifstream & in) {
	if (in >> number)
		return;
	throw std::exception("Bad input stream");
}

void Factorizator::calculate() {
	uint64_t now = number;
	for (uint32_t i = 2; i <= sqrt(now); ++i) {
		if (now % i == 0) {
			factors.push_back(std::make_pair(i, 0));
			while (now % i == 0) {
				factors.back().second++;
				now /= i;
			}
		}
	}
	if (now != 1) 
		factors.push_back(std::make_pair(now, 1));
	if (number == 1)
		factors.push_back(std::make_pair(1, 1));
	if (number == 0)
		ans = "0 = 0";
	if (number == 1)
		ans = "1 = 1";
	std::string answer = std::to_string(number) + " = ";
	size_t sz = factors.size();
	for (uint32_t i = 0; i < sz; ++i) {
		answer += std::to_string(factors[i].first) + (factors[i].second > 1 ? "^" + std::to_string(factors[i].second) : "") + (i == sz - 1 ? "" : " * ");
	}
	ans = answer;
}

std::string Factorizator::get_answer() {
	return ans;
}

void Factorizator::print_answer(std::ofstream & out) {
	out << ans << '\n';
}

