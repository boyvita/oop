#pragma once

template <class T>
Vector<T>::Vector() {
	_size = _capacity = 0;
	data = new T[0];
	//data = static_cast<T*>(operator new[](0));
}

template <class T>
Vector<T>::~Vector() {
	for (uint32_t i = 0; i < _size; ++i)
		data[i].~T();
	delete[](data);
}

template <class T>
void Vector<T>::reserve(uint32_t const& _sz) {
	uint32_t new_capacity;
	if (_capacity < _sz)
		new_capacity = _sz;
	T* new_data = new T[new_capacity];
	//T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
	for (uint32_t i = 0; i < _size; ++i)
		new_data[i] = data[i];
	for (uint32_t i = 0; i < _size; ++i)
		data[i].~T();
	delete[] data;
	data = new_data;
	_capacity = new_capacity;
}

template <class T>
void Vector<T>::resize(uint32_t const& sz) {
	reserve(sz);
	for (; _size < sz; ++_size)
		data[_size] = T();
	_size = sz;
}

template <class T>
T & Vector<T>::at(uint32_t const& num) {
	if (_size < num)
		throw std::range_error("can't address to member");
	else
		return data[num];
}

template <class T>
T & Vector<T>::operator [] (uint32_t const& num) {
	return data[num];
}

template <class T>
Vector<T>::Vector(Vector<T> const& v) {
	data = v.data;
	_size = v._size;
	_capacity = v._capacity;
}

template <class T>
Vector<T> & Vector<T>::operator = (Vector<T> const& v) {
	data = v.data;
	_size = v._size;
	_capacity = v._capacity;

}

template <class T>
void Vector<T>::push_back(T const& val) {
	if (_size + 1 > _capacity)
		reserve((_size + 1) * 2);
	data[_size++] = val;
}

template <class T>
Vector<T>::Vector(Vector<T> && v) {
	swap(data, v.data);
	swap(_size, v._size);
	swap(_capacity, v._capacity);
}

template <class T>
Vector<T> & Vector<T>::operator = (Vector && v) {
	swap(data, v.data);
	swap(_size, v._size);
	swap(_capacity, v._capacity);
}

template <class T>
template <class... Args>
void Vector<T>::emplace_back(Args&&... args) {
	if (_size + 1 > _capacity)
		reserve((_size + 1) * 2);
	data[_size++] = T(forward<Args>(args)...);
}

