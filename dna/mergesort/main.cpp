#include <iostream>
#include <vector>

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

void mergesort(int* array, int* l, int* r)
{
	if (r == l) // termination condition for recursion
		return; 

	std::cout << "[" << l-array << ", " << r-array << "]" << std::endl;

	int size = r-l+1;
	int half_index = size/2;

	std::cout << "Size is: " << size << std::endl;
	std::cout << "Half is: " << half_index << std::endl;

	mergesort(array, l, l+half_index-1);
	mergesort(array, l+half_index, r);

	// TODO: how do we best avoid reallocating heap memory? I could take care 
	// of memory-management myself through a memory pool.
	int *tmp = new int[size];
	int *i=l, *j=l+half_index, k=0;
	while (i<l+half_index && j<=r)
	{
		if (*i < *j)
			tmp[k] = *(i++);
		else
			tmp[k] = *(j++);
		++k;
	}
	while (i<l+half_index)
		tmp[k++] = *(i++);
	while (j<=r)
		tmp[k++] = *(j++);

	std::copy(tmp, tmp+size, l);

	delete [] tmp;
}

int main(int argc, char* argv[])
{
	size_t input_size = 1000;
	std::vector<int> input(1000);
	for (size_t i=0; i<input_size; ++i)
		input[i] = rand()%100;

	std::cout << "Input:  " << input << std::endl;
	mergesort(input.data(), input.data(), input.data()+input.size()-1);
	std::cout << "Output: " << input << std::endl;

	return 0;
}

