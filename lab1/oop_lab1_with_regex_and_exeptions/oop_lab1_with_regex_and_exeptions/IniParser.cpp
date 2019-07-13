#pragma once
#include "IniParser.h"
#include "Exeptions.h"
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
#include <regex>

using namespace std;

bool IniParser::is_double(string const& s) {
	return regex_match(s.c_str(), regex("^([-+]?[\\d]+\\.[\\d]+)|([-+]?\\.[\\d]+)|([-+]?[\\d]+)$"));
}

bool IniParser::is_int(string const& s) {
	return regex_match(s.c_str(), regex("^([-+]?[\\d]+)$"));
}

bool IniParser::is_name_correct(string const& name) {
	return regex_match(name.c_str(), regex("^[0-9a-zA-Z]+$"));
}

bool IniParser::is_string_correct(string const& s) {
	return regex_match(s.c_str(), regex("^[0-9a-zA-Z\\._\\\\\\/]+$"));
}

bool IniParser::has_symbols(string const& s) {
	return regex_search(s.c_str(), regex("[^ \t]"));
}

string IniParser::cut_spaces(string s) {
	return regex_replace(regex_replace(s.c_str(), regex("^[ ]+"), ""), regex("[ ]+$"), "");
}

const vector<string> & IniParser::get_data() const {
	return data;
}

const map<string, map<string, string>> & IniParser::get_table() const {
	return table;
}

void IniParser::compile() {
	vector<pair<int, string>> error_list;

	ifstream in(path.c_str());
	string line;
	int line_num = -1;
	string section_name = "";
	map<string, string> section;
	if (!isOpen)
		throw exc_io("file wasn't opened");

	for (auto line : get_data()) {
		line_num++;
		//deleting comments
		size_t ls = line.size();
		size_t lf1 = line.find(";");
		size_t lf2 = line.find("#");
		line = line.substr(0, min(ls, min(lf1, lf2)));

		//empty string
		if (!has_symbols(line))
			continue;

		//section parsing
		size_t open_bracket = line.find('['), close_bracket = line.find_last_of(']');
		if (open_bracket != string::npos || close_bracket != string::npos) {
			if (section_name != "")
				table[section_name] = section;
			if (!open_bracket == string::npos) {
				error_list.push_back(make_pair(line_num, "missed [ in defenition of section"));
				continue;
			}
			if (!close_bracket == string::npos) {
				error_list.push_back(make_pair(line_num, "missed ] in defenition of section"));
				continue;
			}
			for (size_t i = 0; i < open_bracket; i++)
				if (!isspace(line[i])) {
					error_list.push_back(make_pair(line_num, "incorrect symbol before defenition of section"));
					continue;
				}
			section_name = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
			if (!is_name_correct(section_name)) {
				error_list.push_back(make_pair(line_num, "incorrect name of section"));
				continue;
			}
			for (size_t i = 0; i < open_bracket; i++)
				if (!isspace(line[i])) {
					error_list.push_back(make_pair(line_num, "incorrect symbol after defenition of section"));
					continue;
				}
			if (table.find(section_name) == table.end()) {
				section.clear();
			}
			else {
				error_list.push_back(make_pair(line_num, "reduplicating of section"));
				continue;
			}
			continue;
		}

		//member parsing
		size_t equal_sign = line.find('=');
		if (line.find('=', equal_sign + 1) != string::npos)
			error_list.push_back(make_pair(line_num, "too many symbols of equality"));
		if (section_name == "")
			error_list.push_back(make_pair(line_num, "definition of member without section"));
		if (equal_sign != string::npos) {
			string name_of_member = cut_spaces(line.substr(0, equal_sign));
			if (!is_name_correct(name_of_member)) {
				error_list.push_back(make_pair(line_num, "incorrect member's name"));
				continue;
			}
			string value = cut_spaces(line.substr(equal_sign + 1, line.size() - equal_sign + 1));
			if (!is_string_correct(value)) {
				error_list.push_back(make_pair(line_num, "incorrect value in definition of member"));
				continue;
			}
			if (section.find(name_of_member) == section.end())
				section[name_of_member] = value;
			else {
				error_list.push_back(make_pair(line_num, "reduplicating of member"));
				continue;
			}
			continue;
		}

		error_list.push_back(make_pair(line_num, "unknown semantic"));
		//cout << line << '\n';
	}
	table[section_name] = section;
	if (!error_list.empty())
		throw exc_compile(std::move(error_list));
	isCompile = 1;
}

IniParser::IniParser() {}

IniParser::~IniParser() {}

IniParser::IniParser(string const& path) {
	if (path.substr(path.find('.') + 1, path.size() - path.find('.') + 1) != "ini")
		throw exc_io("wrong format");
	ifstream in(path.c_str());
	if (!in)
		throw exc_io("file wasn't found");
	data.clear();
	string line;
	while (getline(in, line)) {
		data.push_back(line);
	}
	isOpen = 1;
}

template <>
int IniParser::get<int>(string const& section, string const& member) {
	if (!isOpen)
		throw exc_io("ini-file wasn't open");
	if (!isOpen)
		throw exc_access("ini-file wasn't compile");
	if (table.find(section) == table.end())
		throw exc_search("invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw exc_search("invocation of a nonexistent member");
	if (!is_int(table[section][member]))
		throw exc_cast("convertation string in int was failed");
	else
		return stoi(table[section][member]);
}


template <>
double IniParser::get<double>(string const& section, string const& member) {
	if (!isOpen)
		throw exc_io("ini-file wasn't open");
	if (!isOpen)
		throw exc_access("ini-file wasn't compile");
	if (table.find(section) == table.end())
		throw exc_search("invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw exc_search("invocation of a nonexistent member");
	if (!is_double(table[section][member]))
		throw exc_cast("convertation string in int was failed");
	else
		return stod(table[section][member]);
}

template <>
string IniParser::get<string>(string const& section, string const& member) {
	if (!isOpen)
		throw exc_io("ini-file wasn't open");
	if (!isOpen)
		throw exc_access("ini-file wasn't compile");
	if (table.find(section) == table.end())
		throw exc_search("invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw exc_search("invocation of a nonexistent member");
	return table[section][member];
}