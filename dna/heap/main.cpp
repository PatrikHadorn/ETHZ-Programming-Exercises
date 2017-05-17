#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

template<typename T> class Heap
{
public:
	// ------------------------------------------------------------
	// Constructors
	// ------------------------------------------------------------
	template<typename A> Heap(A const& input)
	{
		size_t length = std::distance(input.begin(), input.end());
		std::cout << "Size: " << length << std::endl;

		m_data = new T[length];
		std::copy(input.begin(), input.end(), m_data);
		m_heap_size = length;
		m_array_size = length;

		// TODO: Find second to last "row"
		size_t depth = std::ceil(std::log2(size()));

		for (int i = (1<<depth)-1; i>=0; --i)
			heapify(i);
	}

	// ------------------------------------------------------------
	// Destructors
	// ------------------------------------------------------------
	~Heap()
	{
		delete [] m_data;
	}

	// ------------------------------------------------------------
	// Queries
	// ------------------------------------------------------------
	size_t size(void) const {return m_heap_size;}
	bool empty(void) const {return (m_heap_size == 0);}

	// ------------------------------------------------------------
	// Iterator
	// ------------------------------------------------------------
	T* begin(void) {return m_data;}
	T* end(void) {return m_data+m_heap_size;}

	// ------------------------------------------------------------
	// Heap Operations
	// ------------------------------------------------------------
	size_t parent(size_t i)
	{
		// TODO: Bounds checking?
		return i>>1;
	}

	size_t left(size_t i)
	{
		// TODO: Bounds checking?
		return ((i+1)<<1)-1;
	}

	size_t right(size_t i)
	{
		// TODO: Bounds checking?
		return ((i+1)<<1);
	}

	T const& max(void) const {return *m_data;}
	T extract_max(void)
	{
		if (empty())
			return *m_data; // TODO: Throw

		std::swap(*m_data, *(m_data+m_heap_size-1));
		--m_heap_size;
		heapify(0);

		return m_data[m_heap_size];	
	}

private:
	size_t m_heap_size = 0;
	size_t m_array_size = 0;
	T* m_data = nullptr;

	// ------------------------------------------------------------
	// Heap Management
	// ------------------------------------------------------------
	// TODO: min max operation as template parameter
	void heapify(size_t i) 
	{
		T& t = m_data[i];
		
		if (left(i) < size())
		{
			T& l = m_data[left(i)];
			if (l > t)
				std::swap(t, l);
		}
		
		if (right(i) < size())
		{
			T& r = m_data[right(i)];
			if (right(i) < size() && r > t)
				std::swap(t, r);
		}

		// Termination Condition
		if (right(i) < size())
			heapify(right(i));
		if (left(i) < size())
			heapify(left(i));
	}
};

int main(int argc, char* argv[])
{
	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11};
	Heap<float> testHeap(vec);

	while (!testHeap.empty())
	{
#if 0
		std::cout << "Heap: \n";
		for (auto i : testHeap)
			std::cout << i << ", ";
		std::cout << "\n\n";
#endif

		std::cout << "Max: " << testHeap.extract_max() << std::endl;
	}

	return 0;
}

