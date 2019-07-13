#include <iostream>
#include <vector>
#include <cmath>
#include "Vector.h"

using namespace std;

class A {
public:
	A() {
		val = 0;
	}
	~A() = default;
	A(int const& v) {
		val = v;
	}
	A & operator = (A && a) {
		val = a.val;
		return *this;
	}
	A & operator = (A const& a) {
		val = a.val;
		return *this;
	}
	int val;
};
int main() {
	int k = 3;
	A a(1);
	Vector<A> v1;
	v1.reserve(2);
	v1.push_back(a);
	v1.emplace_back(move(k));
	cout << a.val << ' ' << k << '\n';
	cout << v1[0].val << ' ' << v1[1].val << '\n';

	return 0;
}