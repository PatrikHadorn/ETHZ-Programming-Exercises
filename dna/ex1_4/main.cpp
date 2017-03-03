#include <iostream>

//==============================================================================
// Array2D
// -------
// Convenience Class holding contiguous memory with a 2d access pattern through
// overloaded operator()
//==============================================================================
template<typename T> class Array2D
{
public:
	Array2D(size_t dimX, size_t dimY) : m_sizeX(dimX), m_sizeY(dimY)
	{
		m_data = new T[m_sizeX*m_sizeY];
	}

	~Array2D()
	{
		delete [] m_data;
		m_data = nullptr;
	}

	T& operator()(size_t x, size_t y) {return m_data[y*m_sizeX+x];}
	T const& operator()(size_t x, size_t y) const {return m_data[y*m_sizeX+x];}

	size_t sizeX(void) const {return m_sizeX;}
	size_t sizeY(void) const {return m_sizeY;}

private:
	size_t const m_sizeX = 0, m_sizeY = 0;
	T* m_data = nullptr;

	// Prevent copying for now
	Array2D(Array2D<T> const& cpy) = delete;
	Array2D<T>& operator=(Array2D<T> const& rhs) = delete;
};

template<typename T> 
std::ostream& operator<<(std::ostream& os, Array2D<T> const& array2d)
{
	os << "\n================================================================================\n";
	for (size_t y=0; y<array2d.sizeY(); ++y)
	{
		for (size_t x=0; x<array2d.sizeX(); ++x)
			os << array2d(x, y) << "\t";
		os << "\n";
	}
	os << "================================================================================\n";
	return os;
}

//==============================================================================
// PrefixSum
// ---------
// Accelleration structure for constant-time queries of sums of rectangular 
// subdomains of the input matrix
// 
// The element at location (x, y) represents the sum of the subdomain 
// (0, 0)-(x, y). This allows constant-time queries by building arbitrary sub-
// domains through a combination of subdomains starting at (0,0)
//==============================================================================
template<typename T> class PrefixSum
{
public:
	PrefixSum(Array2D<T> const& input) : 
		m_prefixSum(input.sizeX(), input.sizeY())
	{
		T sum = T(0);
		// Handle first row
		// Simple prefix sum
		for (size_t x=0; x<input.sizeX(); ++x)
			m_prefixSum(x, 0) = sum += input(x, 0);

		// Handle remaining rows
		// Combination of prefix sum of the current row and sum of subdomain
		// (0,0)-(x, y-1)
		for (size_t y=1; y<input.sizeY(); ++y)
		{
			sum = T(0);
			for (size_t x=0; x<input.sizeX(); ++x)
			{
				sum += input(x, y);
				m_prefixSum(x, y) = m_prefixSum(x, y-1) + sum;
			}
		}
	}

	// To calculate the sum of a subdomain (minX, minY)-(maxX, maxY) we can
	// calculate (0,0)-(maxX, maxY), subtract the overaccounted subdomains
	// (0, 0)-(maxX, minY-1) and (0, 0)-(minX-1, maxY) and add back the 
	// intersection of the two (0, 0)-(minX-1, minY-1) which has been 
	// subtracted twice.
	T sum(size_t minX, size_t minY, size_t maxX, size_t maxY) const
	{
		if (minX == 0 && minY == 0)
			return m_prefixSum(maxX, maxY);
		else if (minX == 0)
			return m_prefixSum(maxX, maxY) - m_prefixSum(maxX, minY-1);
		else if (minY == 0)
			return m_prefixSum(maxX, maxY) - m_prefixSum(minX-1, maxY);
		else
			return 
				m_prefixSum(maxX, maxY) - 
				m_prefixSum(maxX, minY-1) - 
				m_prefixSum(minX-1, maxY) +
				m_prefixSum(minX-1, minY-1);
	}

private:
	Array2D<T> m_prefixSum;
};

int main(int argc, char* argv[])
{
	// Get Dimensions
	size_t sizeX, sizeY;
	std::cin >> sizeY >> sizeX;

	// Read in Matrix
	Array2D<int> inputMatrix(sizeX, sizeY);
	for (size_t y=0; y<inputMatrix.sizeY(); ++y)
		for (size_t x=0; x<inputMatrix.sizeX(); ++x)
			std::cin >> inputMatrix(x, y);

	// Read in Subdomain
	size_t minX, minY, maxX, maxY;
	std::cin >> minY >> maxY >> minX >> maxX;

	// Precompute and query
	PrefixSum<int> prefixSum(inputMatrix);
	std::cout << prefixSum.sum(minX, minY, maxX, maxY) << "\n";

	return 0;
}

