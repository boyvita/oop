#include <iostream>
#include "class.h"
#include "generator.h"
#include <string>
#include <fstream>

using namespace std;



int main() {
	string input_file, output_file;
	cout << "Print input file: ";
	cin >> input_file;
	generate(input_file, 10000);
	cout << "Print output file: ";
	cin >> output_file;

	ifstream in(input_file.c_str());
	ofstream out(output_file.c_str());
	ofstream debug("debug.log");
	uint64_t number;
	while (in >> number) { 
		Factorizator fact(number);
		debug << "calculating of number " << number << "...\n";
		fact.calculate();
		debug << "Success\n";
		out << fact.get_answer() << '\n';
	}
	return 0;
}