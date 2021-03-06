#include <iostream>
#include <vector>
#include <list>
#include <cmath>

std::ostream& operator<<(std::ostream& os, std::vector<int> const& v)
{
	size_t size = v.size();
	if (size == 0)
		return os << "{}";

	os << "{";
	for (size_t i=0; i<size-1; ++i)
		os << v[i] << ",";
	os << v[size-1] << "}";

	return os;
}

// TODO: Currently requires positive input, could add a positive/negative bucket at the end?
void bucketsort(int* array, int* l, int* r)
{
	// TODO: Might be better to create an array with n elements and storing the 
	// end of each bucket as an index into that array.
	
	int const num_buckets = 10;
	std::vector<int> buckets[num_buckets];
	for (int i=0; i<num_buckets; ++i)
		buckets[i].reserve((r-l)/num_buckets);

	int max = *l;
	for (int* p = l+1; p<=r; ++p)
		max = std::max(max, *p);
	if (max == 0)
		return;
	int const max_k = std::log10(max);

	int cur_div = 1;
	for (int k=0; k<=max_k; ++k)
	{
		// Filling buckets
		for (int* p=l; p<=r; ++p)
			buckets[(*p/cur_div)%10].push_back(*p);

		// Reading buckets back out
		int array_i = 0;
		for (int i=0; i<num_buckets; ++i)
		{
			for (auto e : buckets[i])
				array[array_i++] = e;
		}

		//std::cout << "Array: {";
		//for (int *p = l; p < r; ++p)
		//	std::cout << *p << ",";
		//std::cout << *r << "}" << std::endl;

		for (int i=0; i<num_buckets; ++i)
			buckets[i].clear();
		cur_div *= 10;
	}
}

int main(int argc, char* argv[])
{
	size_t const input_size = 10000;
	std::vector<int> input(input_size);
	for (size_t i=0; i<input_size; ++i)
		input[i] = rand()%1000000;

	std::cout << "Input:  " << input << std::endl;
	clock_t start = clock();
	bucketsort(input.data(), input.data(), input.data()+input_size-1);
	clock_t end = clock();
	std::cout << "Output: " << input << std::endl;
	std::cout << "Time: " << 1e3*static_cast<double>(end-start)/CLOCKS_PER_SEC << "ms" << std::endl;

	return 0;
}

