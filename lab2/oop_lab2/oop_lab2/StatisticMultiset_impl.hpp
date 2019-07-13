#pragma once
#include <algorithm>
#include <iostream>
#include <typeinfo.h>


template<class T>
StatisticMultiset<T>::StatisticMultiset() {}

//
//template<class T>
//template<class A>
//StatisticMultiset<T>::StatisticMultiset(const StatisticMultiset<A>& statisticMultiset) {
//	if ((A).info() == (T).info()) {
//		data = statisticMultiset.data;
//		CountAboveCashe = statisticMultiset.CountAboveCashe;
//		CountUnderCashe = statisticMultiset.CountUnderCashe;
//	}
//	else {
//		for (auto it : statisticMultiset.data) {
//			data.insert(static_cast<T>(it));
//		}
//		for (auto it : statisticMultiset.countAboveCashe) {
//			countAboveCashe.insert(static_cast<T>(it));
//		}
//		for (auto it : statisticMultiset.countUnderCashe) {
//			countUnderCashe.insert(static_cast<T>(it));
//		}
//	}
//	MinCashe = static_cast<T>(statisticMultiset.MinCashe);
//	MaxCashe = static_cast<T>(statisticMultiset.MaxCashe);
//	AvgCashe = statisticMultiset.AvgCashe;
//}

template<class T>
StatisticMultiset<T>::~StatisticMultiset() {}

//template<class T>
//void StatisticMultiset<T>::print() const {
//	for (auto it : data)
//		std::cerr << it << ' ';
//	std::cerr << '\n';
//}

template<class T>
//template<class A>
void StatisticMultiset<T>::push(const T& val) {
	if (data.empty()) {
		MinCashe = static_cast<T>(val);
		MaxCashe = static_cast<T>(val);
		AvgCashe = static_cast<float>(val);
	}
	else {
		MinCashe = min(MinCashe, static_cast<T>(val));
		MaxCashe = max(MaxCashe, static_cast<T>(val));
		AvgCashe = (AvgCashe * static_cast<float>(data.size()) + static_cast<float>(val)) / (static_cast<float>(data.size()) + 1);
		CountUnderCashe.clear();
		CountAboveCashe.clear();
	}
	data.insert(static_cast<T>(val));
}


template<class T>
//template<class A>
void StatisticMultiset<T>::AddNum(const T& num) {
	push(num);
}


template<class T>
//template<class A>
void StatisticMultiset<T>::AddNum(const std::vector<T>& numbers) {
	for (auto it : numbers)
		push(it);
}


template<class T>
//template<class A>
void StatisticMultiset<T>::AddNums(const StatisticMultiset<T>& a_stat_set) {
	if (a_stat_set.data.empty())
		return;
	if (!data.empty()) {
		MinCashe = min(MinCashe, static_cast<T>(a_stat_set.MinCashe));
		MaxCashe = max(MaxCashe, static_cast<T>(a_stat_set.MaxCashe));
		AvgCashe = (AvgCashe * (int) data.size() + a_stat_set.AvgCashe * (int) a_stat_set.data.size()) / ((int) data.size() + (int) a_stat_set.data.size());
		for (auto it : a_stat_set.data)
			data.insert(it);
	}
	else {
		*this = StatisticMultiset<T>(a_stat_set);
	}
	CountUnderCashe.clear();
	CountAboveCashe.clear();
}

template<class T>
void StatisticMultiset<T>::AddNumsFromFile(const char* filename) {
	ifstream in(filename);
	if (!in)
		throw "file wasn't found";
	T number;
	while (in >> number)
		push(number);
}

template<class T>
T StatisticMultiset<T>::GetMin() const {
	if (data.empty())
		throw "Empty multiset";
	return MinCashe;
}

template<class T>
T StatisticMultiset<T>::GetMax() const {
	if (data.empty())
		throw "Empty multiset";
	return MaxCashe;
}

template<class T>
float StatisticMultiset<T>::GetAvg() const {
	if (data.empty())
		throw "Empty multiset";
	return AvgCashe;
}

template<class T>
int StatisticMultiset<T>::GetCountUnder(float threshold) const {
	if (data.empty())
		throw "Empty multiset";
	auto lb_it = data.lower_bound(static_cast<T>(threshold));
	if (lb_it == data.end())
		return data.size();
	T lb_val = *lb_it;
	auto ans_it = CountUnderCashe.find(lb_val);
	if (ans_it != CountUnderCashe.end())
		return ans_it->second;
	int count = 0;
	for (; lb_it != data.begin(); ++count, --lb_it);
	CountUnderCashe[lb_val] = count;
	return count;
}


template<class T>
int StatisticMultiset<T>::GetCountAbove(float threshold) const {
	if (data.empty())
		throw "Empty multiset";
	auto ub_it = data.upper_bound(static_cast<T>(threshold));
	if (ub_it == data.end())
		return 0;
	T ub_val = *ub_it;
	auto ans_it = CountAboveCashe.find(ub_val);
	if (ans_it != CountAboveCashe.end())
		return ans_it->second;
	int count = 0;
	for (; ub_it != data.end(); ++count, ++ub_it);
	CountUnderCashe[ub_val] = count;
	return count;
}
