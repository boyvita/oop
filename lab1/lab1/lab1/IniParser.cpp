#pragma once
#include "IniParser.h"
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

using namespace std;	

bool IniParser::is_double(string const& s) {
	bool haspoint = 0;
	if (s.size() > 1 && s[0] == '0' && s[1] == '0')

		for (int i = 0; i < (int) s.size(); i++)
			if (!isdigit(s[i]))
				if (s[i] == '.' && !haspoint)
					haspoint = 1;
				else
					return false;
	return true;
}

bool IniParser::is_int(string const& s) {
	if (s[0] == '0')
		return false;
	for (int i = 0; i < (int) s.size(); i++)
		if (!isdigit(s[i]))
			return false;
	return true;
}

bool IniParser::is_name_correct(string const& name) {
	if (name.empty())
		return false;
	for (int i = 0; i < (int) name.size(); i++)
		if (!isalnum(name[i]))
			return false;
	return true;
}

bool IniParser::is_string_correct(string const& s) {
	if (s.empty())
		return false;
	for (int i = 0; i < (int) s.size(); i++)
		if (!isalnum(s[i]) && s[i] != '.')// && s[i] != '/' && s[i] != '_');
			return false;
	return true;
}

bool IniParser::has_symbols(string const& s) {
	for (int i = 0; i < (int) s.size(); i++)
		if (!isspace(s[i]))
			return true;
	return false;
}

string IniParser::cut_spaces(string s) {
	if (!has_symbols(s))
		return "";
	int beg, en;
	for (int i = 0; i < (int) s.size(); i++)
		if (!isspace(s[i])) {
			beg = i;
			break;
		}
	for (int i = (int) s.size() - 1; i >= 0; i--)
		if (!isspace(s[i])) {
			en = i;
			break;
		}
	return s.substr(beg, en - beg + 1);
}

vector<string> IniParser::get_data() const {
	return data;
}

map<string, map<string, string>> IniParser::get_table() const {
	return table;
}

void IniParser::compile(bool ignore_compile_errors) {
	vector<pair<int, const char*>> error_list;

	ifstream in(path.c_str());
	string line;
	int line_num = -1;
	string section_name = "";
	map<string, string> section;
	if (!isOpen)
		throw "file wasn't opened";

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
		size_t open_bracket = line.find('['), close_bracket = line.find(']');
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
			for (int i = 0; i < (int) open_bracket; i++)
				if (!isspace(line[i])) {
					error_list.push_back(make_pair(line_num, "incorrect symbol before defenition of section"));
					continue;
				}
			section_name = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
			if (!is_name_correct(section_name)) {
				error_list.push_back(make_pair(line_num, "incorrect name of section"));
				continue;
			}
			for (int i = 0; i < (int) open_bracket; i++)
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
	if (!ignore_compile_errors)
		if (!error_list.empty())
			throw error_list;
	isCompile = 1;
}

IniParser::IniParser() {}

IniParser::~IniParser() {}

IniParser::IniParser(string path) {
	if (path.substr(path.find('.') + 1, path.size() - path.find('.') + 1) != "ini")
		throw make_pair(-1, "wrong format");
	ifstream in(path.c_str());
	if (!in)
		throw make_pair(-1, "file wasn't found");
	data.clear();
	string line;
	while (getline(in, line)) {
		data.push_back(line);
	}
	isOpen = 1;
}

template <>
int IniParser::get<int>(string section, string member) {
	if (!isOpen)
		throw make_pair(-1, "ini-file wasn't open");
	if (!isOpen)
		throw make_pair(-1, "ini-file wasn't compile");
	if (table.find(section) == table.end())
		throw make_pair(-1, "invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw make_pair(-1, "invocation of a nonexistent member");
	if (!is_int(table[section][member]))
		throw make_pair(-1, "convertation string in int was failed");
	else
		return stoi(table[section][member]);
}


template <>
double IniParser::get<double>(string section, string member) {
	if (table.find(section) == table.end())
		throw make_pair(-1, "invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw make_pair(-1, "invocation of a nonexistent member");
	if (!is_double(table[section][member]))
		throw make_pair(-1, "convertation string in int was failed");
	else
		return stod(table[section][member]);
}

template <>
string IniParser::get<string>(string section, string member) {
	if (table.find(section) == table.end())
		throw make_pair(-1, "invocation of a nonexistent section");
	if (table[section].find(member) == table[section].end())
		throw make_pair(-1, "invocation of a nonexistent member");
	return table[section][member];
}