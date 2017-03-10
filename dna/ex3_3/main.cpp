#include <iostream>
#include <cmath>

#include "MyVector.h"

int main(int argc, char* argv[])
{
	MyVector<int> myVector(10);

	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	std::cout << "[0]: " << myVector[0] << std::endl;
	std::cout << "[9]: " << myVector[9] << std::endl;

	try
	{
		std::cout << "at(0): " << myVector.at(0) << std::endl;
		std::cout << "at(9): " << myVector.at(9) << std::endl;
		std::cout << "at(10): " << myVector.at(10) << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	for (auto el : myVector)
		std::cout << el << ", ";
	std::cout << std::endl;

	std::cout << "Clearing\n";
	myVector.clear();
	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;
	std::cout << "Shrinking to fit\n";
	myVector.shrink_to_fit();
	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	for (int i=0; i<33; ++i)
		myVector.push_back(i);

	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	for (auto el : myVector)
		std::cout << el << ", ";
	std::cout << std::endl;

	std::cout << "Last Element: " << myVector.back() << std::endl;

	for (int i=0; i<33; ++i)
		myVector.pop_back();

	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	myVector.shrink_to_fit();

	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	std::cout << "================================================================================\n";
	myVector.reserve(80);
	std::cout << "empty(): " << myVector.empty() << std::endl;
	std::cout << "size(): " << myVector.size() << std::endl;
	std::cout << "capacity(): " << myVector.capacity() << std::endl;

	return 0;
}

