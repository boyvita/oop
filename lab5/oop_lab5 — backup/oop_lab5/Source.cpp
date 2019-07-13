#include <iostream>
#include "ThreadProcessor.h"
#include "generator.h"
#include "Factorizator.h"
#include <string>
#include <fstream>
using namespace std;

int main() {
	string input_file, output_file;
	uint32_t count_threads, pool_max_size;
	cout << "Print input file: ";
	cin >> input_file;
	//input_file = "a.in";

	cout << "Print output file: ";	
	cin >> output_file;
	//output_file = "a.out";

	//generate(input_file, 10000);

	cout << "Print max count of threads: ";
	cin >> count_threads;
	
	cout << "Print max length of threadpool: ";
	cin >> pool_max_size;
	
	ThreadProcessor<Factorizator> tp(input_file, output_file, count_threads, pool_max_size);
	tp.start();

	return 0;
}
