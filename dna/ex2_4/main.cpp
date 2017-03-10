#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

template<typename T> class Selector
{
public:
	using value_type = typename T::value_type;
	using container_type = T;
	using iterator_type = typename T::iterator;

public:
	Selector(T const& a) : m_array(a) {};

	value_type selectRandomize(size_t k);
	value_type selectBlum(size_t k);
	value_type selectBruteForce(size_t k);

private:
	T m_array;

	// Pre: pivot is an element in the range [first, last)
	// Post: Elements that compare '<' to pivot element are to the left of the pivot
	iterator_type partition(
			iterator_type first, iterator_type last, value_type const pivot)
	{
		// Find Pivot position
		auto pivotIt = std::find(first, last+1, pivot);
		assert(pivotIt != last);

		// Move Pivot to right
		std::swap(*pivotIt, *last);

		auto writeIter = first;
		for (auto it = first; it != last; ++it)
		{
			if (*it < pivot)
			{
				std::swap(*writeIter, *it); // TODO: Unnecessary swaps
				++writeIter;
			}
		}

		// Move Pivot in-between
		std::swap(*last, *writeIter);
		return writeIter;
	}

	template<typename Func> int select(size_t k, Func pivotStrategy)
	{
		auto first = m_array.begin(), last = m_array.end()-1;

		while (first != last)
		{
			auto pivot = pivotStrategy(first, last, k);
			auto pivotIt = partition(first, last, pivot);

			if (pivotIt-m_array.begin() == k)
				return *pivotIt;

			if (k < pivotIt - m_array.begin())
				last = pivotIt-1;
			else
				first = pivotIt+1;
		}
		return *first;
	}

	static value_type simplePivot(iterator_type first, iterator_type last, int index)
	{
		return *first;
	}
	
	static value_type randomPivot(iterator_type first, iterator_type last, int index)
	{
		auto pivotIt = first;
		int count;
		do {
			count = 0;
			auto const pivot = *pivotIt;
			for (auto it = first; it != last; ++it)
				count += (*it < pivot);
			++pivotIt;
		}
		while (pivotIt < last && (count < (last-first+1)/4 || count > 3*(last-first+1)/4));

		return *(pivotIt-1);
	}
};

template<typename T> typename Selector<T>::value_type 
Selector<T>::selectRandomize(size_t k)
{
	return select(k, randomPivot);
}

template<typename T> typename Selector<T>::value_type 
Selector<T>::selectBlum(size_t k)
{
	// Calculate Median of Groups of 5
	//std::vector<int> medians(m_array.size()/5+1);
	//int tmp[5];
	//for (size_t j=0; j<m_array.size(); j+=5)
	//{
	//	std::copy(&m_array[j], &m_array[j+5], tmp);
	//	std::sort(tmp, tmp+5);
	//	medians[j/5] = tmp[2];
	//}

	//std::cout << "Medians: [";
	//for (int el : medians)
	//	std::cout << el << " ";
	//std::cout << "]\n";

	// Calculate Median of Medians
	return 0;
}

template<typename T> typename Selector<T>::value_type 
Selector<T>::selectBruteForce(size_t k)
{
	// Whatever's meant by brute force
#if 1
	return select(k, simplePivot);
#else
	std::sort(m_array.begin(), m_array.end());
	return m_array[k];
#endif
}


int main(int argc, char* argv[])
{
	int size;
	std::cin >> size;
	std::vector<int> array(size);
	for (int i=0; i<size; ++i)
		std::cin >> array[i];

	int selection;
	std::cin >> selection;
	std::cout << "Selection: " << selection << std::endl;

	Selector<std::vector<int>> selector1(array);
	std::cout << "Brute Force: " << selector1.selectBruteForce(selection) << std::endl;

	Selector<std::vector<int>> selector2(array);
	std::cout << "Randomize: " << selector2.selectRandomize(selection) << std::endl;

	return 0;
}

