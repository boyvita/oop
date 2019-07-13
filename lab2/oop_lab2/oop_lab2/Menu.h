#pragma once
#include <map>
#include <vector>
#include <string>

using namespace std;

class Menu {
	string name;
	string text;
	map <string, Menu> tree;
	class Action {
		string text;
		template<typename... Ts> void(*function)(Ts...);
	public:
		template<typename... Ts> 
		Action(string text = "", void(*function)(Ts...) = lambda(Ts...) {}) : text(text), function(function) {}
	};
	vector <Action> actions;
	void(*additional)();

public:
	Menu(string name, string text, void(*additional)()) {}
};