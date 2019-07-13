#pragma once
#include <stdint.h>
#include <exception>
#include <stdexcept>

template <typename T>
class Vector {
public:
	Vector();

	~Vector();

	void reserve(uint32_t const& _sz);
	
	void resize(uint32_t const& sz);

	T & at(uint32_t const& num);

	T & operator [] (uint32_t const& num);

	Vector(Vector<T> const& v);
	
	Vector<T> & operator = (Vector<T> const& v);
	
	void push_back(T const& val);

	Vector(Vector<T>&& v);

	Vector<T> & operator = (Vector<T> && v);

	template <typename... Args>
	void emplace_back(Args&&... args);

private:
	T * data;
	uint32_t _size;
	uint32_t _capacity;
};

#include "Vector_impl.hpp"