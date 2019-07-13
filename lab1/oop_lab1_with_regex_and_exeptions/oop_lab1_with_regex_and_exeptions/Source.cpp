#include <iostream>
#include <string>
#include <cstdio>
#include "IniParser.h"

using namespace std;

int main() {
	string path;
	IniParser ip;
	cout << "IniParser (c) Vitaly Boytsov M3207\nall rights reserved ;)\n";
	cout << "possible commands:\n";
	cout << "open/o <path> - open and parse ini-file\n";
	cout << "print/p - print data of immediate ini-file\n";
	cout << "table/t - print resources of ini-file\n";
	cout << "getdouble/gd <name of section> <name of member> - return float-value from ini-file\n";
	cout << "getint/gi <name of section> <name of member> - return integer-value from ini-file\n";
	cout << "getstring/gs <name of section> <name of member> - return string-value from ini-file\n";
	cout << "exit - nothing to explain\n";
	while (true) { 
		string command, s1, s2;
		cin >> command;
		if (command == "open" || command == "o") {
			cin >> s1;
			try {
				ip = IniParser(s1);
				try {
					ip.compile();
					for (auto section : ip.get_table()) {
						cout << "[" << section.first << "]\n";
						for (auto member : section.second)
							cout << member.first << " = " << member.second << '\n';
						cout << '\n';
					}
					cout << "file was opened succesfully\n";
				}
				catch (exc_compile e) {
					vector <string> data = ip.get_data();
					for (size_t i = 0; i < data.size(); i++)
						printf("%2d: %s\n", i, data[i].c_str());
					exc_compile::DataType error_list = e.getData();
					sort(error_list.begin(), error_list.end());
					for (auto error: error_list)
						cout << "Compile error: " << error.second << " in line: " << error.first << '\n';
					cout << "Try open another ini-file\n";
				}
			}
			catch (runtime_error e) {
				cout << "Runtime error: " << e.what() << '\n';
			}
			continue;
		}
		if (command == "print" || command == "p") {
			vector <string> data = ip.get_data();
			if (data.empty())
				cout << "data wasn't found\n";
			else
				for (size_t i = 0; i < data.size(); i++)
					printf("%2d: %s\n", i + 1, data[i].c_str());
			continue;
		}
		if (command == "table" || command == "t") {
			map<string, map<string, string>> table = ip.get_table();
			if (table.empty())
				cout << "table wasn't found\n";
			else {
				for (auto section : table) {
					cout << "[" << section.first << "]\n";
					for (auto member : section.second)
						cout << member.first << " = " << member.second << '\n';
					cout << '\n';
				}
				continue;
			}
		}
		try {
			if (command == "getint" || command == "gi") {
				cin >> s1 >> s2;
				cout << ip.get<int>(s1, s2) << '\n';
				continue;
			}
			if (command == "getdouble" || command == "gd") {
				cin >> s1 >> s2;
				cout << ip.get<double>(s1, s2) << '\n';
				continue;
			}
			if (command == "getstring" || command == "gs") {
				cin >> s1 >> s2;
				cout << ip.get<string>(s1, s2) << '\n';
				continue; 
			}

		}
		catch (runtime_error e) {
			cout << "error: " << e.what() << '\n';
			continue;
		}
		if (command == "exit" || command == "e") {
			break;
		}
		cout << "unknown command\n";

	}
	return 0;
}