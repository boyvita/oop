#pragma once
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

void generate(string file_name, uint32_t count) {
	srand(time(NULL));
	ofstream out(file_name.c_str());
	for (int i = 0; i < count; i++)
		out << uint64_t(rand()) * uint64_t(rand()) * uint64_t(rand()) * uint64_t(rand()) << '\n';
}