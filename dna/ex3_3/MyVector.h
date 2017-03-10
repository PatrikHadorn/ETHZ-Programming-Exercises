#pragma once

#include <stdexcept>
#include <algorithm>

template<typename T> class MyVector
{
public:
	using value_type = T; 
	using iterator_type = T*;

public:
	MyVector(){}
	MyVector(size_t size, value_type const& val=value_type())
	{
		if (size == size_t(0))
			return;

		// find next power-of-two size
		size_t pow_of_two = std::ceil(std::log2(size));
		std::cout << "pow_of_two: " << pow_of_two << std::endl;
		
		m_size = size;
		m_capacity = 1 << pow_of_two;
		std::cout << "Capacity: " << m_capacity << std::endl;
		std::cout << "Size: " << m_size << std::endl;

		m_data = new value_type[m_size];
	}

	iterator_type begin(void) {return m_data;}
	iterator_type end(void) {return m_data+m_size;}

	size_t size(void) const {return m_size;}
	size_t capacity(void) const {return m_capacity;}
	bool empty(void) const {return m_size==size_t(0);}

	void push_back(value_type const& val)
	{
		if (m_size+1 > m_capacity)
		{
			// double capacity
			m_capacity = (m_capacity > size_t(0)) ? m_capacity << 1 : size_t(1);

			value_type* tmp = new value_type[m_capacity];
			std::copy(m_data, m_data+m_size, tmp);
			std::swap(tmp, m_data);
		}

		m_data[m_size++] = val;
	}

	// Calling back on empty container is undefined
	value_type& back(void) {return m_data[m_size-1];}
	value_type const& back(void) const {return m_data[m_size-1];}

	// Subscript operator
	// Fall back to range checked versions in debug mode
#ifdef NDEBUG
	value_type& operator[](size_t i){return m_data[i];}
	value_type const& operator[](size_t i) const {return m_data[i];}
#else
	value_type& operator[](size_t i){return at(i);}
	value_type const& operator[](size_t i) const {return at(i);}
#endif

	// Range checked access operators
	value_type& at(size_t i)
	{
		if (i < size_t(0) || i >= m_size)
			throw std::out_of_range("at() out of range");
		return m_data[i];
	}

	value_type const& at(size_t i) const 
	{
		return at(i);
	}

	void pop_back(void) {--m_size;}

	// Mimicing std::vector, clear does not free up memory
	void clear(void)
	{
		m_size = size_t(0);
	}

	void shrink_to_fit(void)
	{
		if (m_size == size_t(0))
		{
			delete [] m_data;
			m_data = nullptr;
			m_capacity = size_t(0);
		}
		else
		{
			// find next power-of-two size
			size_t pow_of_two = std::ceil(std::log2(m_size));
			size_t shrinked_capacity = 1 >> pow_of_two;
			if (shrinked_capacity < m_capacity)
			{
				value_type* tmp = new value_type[shrinked_capacity];
				std::swap(m_data, tmp);
				delete [] tmp;
				tmp = nullptr;
				m_capacity = shrinked_capacity;
			}
		}
	}

	void reserve(size_t n)
	{
		if (n > m_capacity)
		{
			size_t newCapacity = 1 << static_cast<size_t>(std::ceil(std::log2(n)));
			value_type *tmp = new value_type[newCapacity];
			std::swap(tmp, m_data);
			delete [] tmp;
			tmp = nullptr;
			m_capacity = newCapacity;
		}
	}

private:
	size_t m_size = size_t(0);
	size_t m_capacity = size_t(0);
	value_type* m_data = nullptr;
};
