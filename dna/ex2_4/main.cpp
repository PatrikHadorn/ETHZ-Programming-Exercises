#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Selector
{
public:
	Selector(std::vector<int> const& a);

	int selectRandomize(size_t k);
	int selectBlum(size_t k);
	int selectBruteForce(size_t k);

private:
	std::vector<int> m_array;

};

Selector::Selector(std::vector<int> const& a) : m_array(a)
{
}

int Selector::selectRandomize(size_t k)
{
	return 0;
}

int Selector::selectBlum(size_t k)
{
	// Calculate Median of Groups of 5
	std::vector<int> medians(m_array.size()/5+1);
	int tmp[5];
	for (int j=0; j<m_array.size(); j+=5)
	{
		std::copy(&m_array[j], &m_array[j+5], tmp);
		std::sort(tmp, tmp+5);
		medians[j/5] = tmp[2];
	}

	std::cout << "Medians: [";
	for (int el : medians)
		std::cout << el << " ";
	std::cout << "]\n";

	// Calculate Median of Medians
	return 0;
}

int Selector::selectBruteForce(size_t k)
{
	return 0;
}

void print_array(std::vector<int> const& array)
{
	std::cout << "[";
	for (auto el : array)
		std::cout << el << " ";
	std::cout << "]\n";
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

	print_array(array);

	Selector selector(array);
	selector.selectBlum(5);

	return 0;
}

