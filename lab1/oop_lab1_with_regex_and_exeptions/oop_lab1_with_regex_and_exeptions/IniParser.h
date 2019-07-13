#pragma once
#include <map>
#include <set>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdio>
#include <cctype>
#include <fstream>
#include <typeinfo.h>
#include "Exeptions.h"

using namespace std;

class IniParser {
private:
	string path;
	vector <string> data;
	bool isOpen;
	bool isCompile;
	map<string, map<string, string>> table;

	bool is_double(string const& s);

	bool is_int(string const& s);

	bool is_name_correct(string const& name);

	bool is_string_correct(string const& s);

	bool has_symbols(string const& s);

	string cut_spaces(string s);

public:
	const vector<string> & get_data() const;

	const map<string, map<string, string>> & get_table() const;

	void compile();

	IniParser();

	~IniParser();

	IniParser(string const& path);

	template <class T>
	T get(string const& section, string const& member);
};
