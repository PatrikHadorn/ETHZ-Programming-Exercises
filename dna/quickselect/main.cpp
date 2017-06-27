#include <iostream>
#include <vector>
#include <algorithm>

void print_array(int* array, size_t size)
{
	std::cout << "[";
	for (size_t i=1; i<size; ++i)
		std::cout << *(array++) << ", ";
	std::cout << *array << "]";
}

int* pick_pivot(int* array, int* l, int* r)
{
	int* p = l-1;
	int counter;
	do {
		++p;
		counter = 0;
		for (int* tmp = l; tmp<=r; ++tmp)
			if (*tmp < *p)
				++counter;
	} while (counter < (r-l)/4 || counter > 3*(r-l)/4);

	return p;
}

int* pivot(int* array, int* l, int* r)
{
	int const pivot_i = pick_pivot(array, l, r)-array;
	int const pivot_val = array[pivot_i];
	//std::cout << "Pivot: " << pivot_val << std::endl;

	std::swap(array[pivot_i], *r);
	int* const pivot_el = r;

	print_array(array, 10);
	std::cout << "\n";
	while (r != l)
	{
		if (*l > pivot_val)
		{
			std::swap(*l, *(--r));
			//print_array(array, 10);
			//std::cout << "\n";
		}
		else
			l++;
	}
	std::swap(*r, *pivot_el);
	return r;
}

int* quickselect(int k, int* array, int* l, int* r)
{
	if (l == r)
		return l;

	std::cout << "\n";
	print_array(array, 10);
	std::cout << "\n";
	int* p = pivot(array, l, r);
	print_array(array, 10);
	std::cout << "\n";
	std::cout << "Pivot: " << *p << std::endl;
	std::cout << "l: " << *l << ", r: " << *r << std::endl;
	std::cout << "p index: " << p-array << std::endl;

	if (p-array+1 == k)
		return p;
	else if (p-array+1 < k)
	{
		std::cout << "Pivoting around (r) " << *(p+1) << " and " << *r << std::endl;
		return quickselect(k, array, ++p, r);
	}
	else
	{
		std::cout << "Pivoting around (l) " << *l << " and " << *(p-1) << std::endl;
		return quickselect(k, array, l, --p);
	}
}

void test(int numRuns = 10000)
{
	srand(time(0));

	int array[10];
	for (int i=0; i<numRuns; ++i)
	{
		for (int i=0; i<10; ++i)
			array[i] = i;
		for (int i=0; i<10; ++i)
			std::swap(array[i], array[rand()%10]);

		std::cout << "================================================================================\n";
		int order = rand()%9+1;
		std::cout << "Order: " << order << std::endl;

		int* result = quickselect(order, array, array, array+9);

		if (*result != order-1)
		{
			std::cout << "Error" << std::endl;
			break;
		}
		std::cout << "================================================================================\n";
	}
}

int main(int argc, char* argv[])
{
	srand(time(0));

	int array[10];
	for (int i=0; i<10; ++i)
		array[i] = i;
	for (int i=0; i<10; ++i)
		std::swap(array[i], array[rand()%10]);

	std::cout << "Input: ";
	print_array(array, 10);
	std::cout << "\n";

	//pivot(array, array, array+9);

	//std::cout << "Output: ";
	//print_array(array, 10);
	//std::cout << "\n";
	
	int* result = quickselect(5, array, array, array+9);
	std::cout << "Result: " << *result << ", Index: " << result-array << std::endl;

	test();

	return 0;
}

