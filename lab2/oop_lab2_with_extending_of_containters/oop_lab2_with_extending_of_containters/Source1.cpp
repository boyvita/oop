#include <iostream>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;


template<class T>
class myset : public multiset<T> {
public:
	typedef multiset<T> MyBase;
	using multiset<T>::multiset();

	void insert(T const& val) {
		push(val);
		MyBase::insert(val);
	}
	void insert(const std::vector<T>& numbers) {
		for (auto i : numbers) {
			insert(i);
		}
	}

	void insert(const myset<T>& a_stat_set) {
		for (auto i : a_stat_set)
			insert(i);
	}

	//void insert(ifstream f) {
	//	istream_iterator<T> input_iterator(f);
	//	istream_iterator<T> end_of_stream();
	//	while (input_iterator != end_of_stream) {
	//		insert(*input_iterator);
	//	}
	//}

	T getMin() const {
		return minCashe;
	}

	T getMax() const {
		return maxCashe;
	}

	float getAvg() const {
		return avgCashe;
	}


private:
	mutable T minCashe, maxCashe;
	mutable float avgCashe;
	mutable std::unordered_map<T, int> CountUnderCashe, CountAboveCashe;

	void push(T const& val) {
		if (empty()) {
			minCashe = val;
			maxCashe = val;
			avgCashe = val;
		}
		else {
			minCashe = min(minCashe, val);
			maxCashe = max(maxCashe, val);
			avgCashe = (avgCashe * static_cast<float>(size()) + static_cast<float>(val)) / (static_cast<float>(size()) + 1);
			CountUnderCashe.clear();
			CountAboveCashe.clear();
		}
	}
};


int main() {
	myset<int> s, s1;
	s.insert(1);
	s.insert(2);
	s1.insert(vector<int>{1, 2, 3});
	s.insert(vector<int>{6, 4, 5});
	s.insert(s1);
	//s.insert(ifstream("file.txt"));
	for (auto i : s)
		cout << i << ' ';
	cout << '\n' << s.getMin() << ' ' << s.getMax() << ' ' << s.getAvg();
}