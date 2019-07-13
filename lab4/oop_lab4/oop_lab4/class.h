#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdint.h>
#include <string>

class Factorizator {
public:
	Factorizator() {};
	~Factorizator() = default;
	Factorizator(uint64_t const& number) : number(number) {}
	void calculate() {
		uint64_t now = number;
		for (uint32_t i = 2; i < sqrt(now); ++i) {
			if (now % i == 0) {
				factors.push_back(std::make_pair(i, 0));
				while (now % i == 0) {
					factors.back().second++;
					now /= i;
				}
			}
		}
	}
	std::string get_answer() {
		std::string answer = std::to_string(number) + " = ";
		size_t sz = factors.size();
		for (uint32_t i = 0; i < sz; ++i) {
			answer += std::to_string(factors[i].first) + (factors[i].second > 1 ? "^" + std::to_string(factors[i].second) : "") + (i == sz - 1 ? "" : " * ");
		}
		return answer;
	}
private:
	uint64_t number;
	std::vector<std::pair<uint32_t, uint8_t>> factors;
};

