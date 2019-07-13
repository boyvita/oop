#include "Exeptions.h"
#include <iostream>
#include <string>
#include <typeinfo>
#include <regex>

using namespace std;

//как устроен аллокатор
//почему нельзя вернуть по ссылке строку
string cut_spaces(string s) { 
	auto beg = s.begin();
	for (; beg != s.end() && isspace(*beg); ++beg);
	auto end = s.rbegin();
	for (; end != s.rend() && isspace(*end); ++end);
	return s;
	//return static_cast<string>(s.assign(beg, end.base()));
}
string
sr1 = "^[-+]?[\\d]+\\.?$",
sr2 = "^[-+]?[\\d]+\\.[\\d]+$",
sr3 = "^[-+]?\\.[\\d]+$";
void is_double(string s) {
	regex rdouble("^([-+]?[\\d]+\\.[\\d]+)|([-+]?\\.[\\d]+)|([-+]?[\\d]+)$");
	cout << regex_match(s.c_str(), rdouble) << ' ' << s << '\n';
}


int main1() {
	cout << sr1 << ' ' << sr2 << ' ' << sr3 <<'\n';
	is_double("0013");
	is_double(".02323");
	is_double("0.2323");
	is_double("1");
	return 0;
}