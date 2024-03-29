#pragma once
#include <set>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>

template<class T>
class StatisticMultiset {
public:
	//void print() const;

	StatisticMultiset();

	/*template<class A>
	StatisticMultiset(const StatisticMultiset<A>& a_stat_set);
*/
	~StatisticMultiset();
	
	void AddNum(const T& num);

	//template<class A, template <class> class C>
	void AddNum(const std::vector<T>& numbers);

	//template<class A> 
	void AddNums(const StatisticMultiset<T>& a_stat_set);

	void AddNumsFromFile(const char* filename);

	T GetMin() const;

	T GetMax() const;

	float GetAvg() const;

	int GetCountUnder(float threshold) const;

	int GetCountAbove(float threshold) const;

private:
	std::multiset<T> data;
	mutable T MinCashe, MaxCashe;
	mutable float AvgCashe;
	mutable std::unordered_map<T, int> CountUnderCashe, CountAboveCashe;

	void push(const T& val);
};


#include "StatisticMultiset_impl.hpp"