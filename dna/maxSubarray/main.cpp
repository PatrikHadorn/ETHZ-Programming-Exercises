#include <iostream>

#include "Array1D.h"

void fillArray(Array1D<int>& array, int min, int max)
{
	//srand48(time(0));
	srand48(1234);
	for (auto& el : array)
		el = min + lrand48()%(max-min+1);
}

void findCenteredSubarray(Array1D<int>& array, int& l, int m, int& r, int& s)
{
	int maxIndex0 = m;
	int maxVal = array[m];
	int cumSum = array[m];
	for (int i=m-1; i>=l; --i)
	{
		cumSum += array[i];
		if (cumSum > maxVal)
		{
			maxVal = cumSum;
			maxIndex0 = i;
		}
	}

	int maxIndex1 = m;
	cumSum = maxVal;
	for (int i=m+1; i<=r; ++i)
	{
		cumSum += array[i];
		if (cumSum > maxVal)
		{
			maxVal = cumSum;
			maxIndex1 = i;
		}
	}

	l = maxIndex0;
	r = maxIndex1;
	s = maxVal;
} 

void maxSubarray(Array1D<int>& array, int& l, int& r, int& s)
{
	if (r == l)
	{
		s = array[l];
		return;
	}

	int m = l + (r-l)/2;
	int l0 = l, l1 = m+1, r0 = m, r1 = r;
	int s0, s1;

	maxSubarray(array, l0, r0, s0);
	maxSubarray(array, l1, r1, s1);
	findCenteredSubarray(array, l, m, r, s);

	// Combine
	if (r0 == m || l0 == m || s > std::max(s0, s1))
		return;
	else
	{
		if (s0 > s1)
		{
			l = l0; 
			r = r0; 
			s = s0;
		}
		else
		{
			l = l1; 
			r = r1; 
			s = s1;
		}
	}
}

int maxSubarray(Array1D<int>& array, int& l, int& r)
{
	if (array.empty())
		return 0;

	l = 0; 
	r = array.size()-1L;
	int s = 0;
	maxSubarray(array, l, r, s);
	return s;
}

int main(int argc, char* argv[])
{
	size_t subarraySize;
	std::cout << "Size of Array to generate: ";
	std::cin >> subarraySize;
	std::cout << std::endl;

	Array1D<int> array(subarraySize);

	fillArray(array, -12, 12);
	std::cout << "Array: " << array << std::endl;

	int l=0, r = 0;
	int sum = maxSubarray(array, l, r);
	std::cout << "MaxSubarray: [" << l << "," << r << "] : " << sum << std::endl;

	return 0;
}
