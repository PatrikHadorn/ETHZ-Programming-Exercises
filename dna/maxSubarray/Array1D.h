#pragma once

#include <iostream>

template<typename T> class Array1D
{
public:
	Array1D(){}
	Array1D(size_t size)
	{
		resize(size);
	}

	~Array1D()
	{
		delete [] m_data;
		m_data = nullptr;
		m_size = 0;
	}

	void resize(size_t size)
	{
		T* tmp = new T[size];
		std::swap(tmp, m_data);
		m_size = size;
		delete [] tmp;
	}

	size_t size(void) const {return m_size;}
	bool empty(void) const {return m_size==size_t(0);}

	void clear(void)
	{
		T* tmp = nullptr;
		std::swap(tmp, m_data);
		m_size = 0;
		delete [] tmp;
	}

	T& operator[](size_t i) {return m_data[i];}
	T const& operator[](size_t i) const {return m_data[i];}

	T* begin(void) {return m_data;}
	T* end(void) {return m_data+m_size;}

	T const* cbegin(void) const {return m_data;}
	T const* cend(void) const {return m_data+m_size;}

private:
	T* m_data = nullptr;
	size_t m_size = 0;
};

template<typename T> 
std::ostream& operator<<(std::ostream& os, Array1D<T> const& rhs)
{
	if (rhs.empty())
		return os << "{}";

	long int size = rhs.size();
	os << "{";
	for (long int i=0; i<size-1L; ++i)
		os << rhs[i] << ",";
	os << rhs[size-1L] << "}";

	return os;
}

