#include <iostream>
#include <cstring>

template <typename T> T sqr(T val)
{
	return val*val;
}

int main(int argc, char* argv[])
{
	std::string input;
	std::cin >> input;

	std::cout << "Input: " << input << std::endl;

	int *dp_table = new int[sqr(input.size()+1)];
	std::memset(dp_table, 0, sizeof(int)*sqr(input.size()+1));

	size_t size = input.size()+1;
	for (int j=1; j<size; ++j)
	{
		for (int i=1; i<size; ++i)
		{
			int diag = dp_table[(j-1)*size+i-1];
			int left = dp_table[j*size+i-1];
			int top = dp_table[j*size+i-size];
			bool match = input[i-1] == input[input.size()-j];

			//std::cout << "Checking match between: " << input[i-1] << " and " << input[input.size()-i] << std::endl;
			//std::cout << "Match: " << match << "\n\n";

			dp_table[j*size+i] = 
				std::max(left, std::max(top, diag+match*1));
		}
	}

	std::cout << "Table: \n";
	for (int j=0; j<size; ++j)
	{
		for (int i=0; i<size; ++i)
		{
			std::cout << dp_table[j*size+i] << " ";
		}
		std::cout << "\n";
	}

	delete [] dp_table;
	return 0;
}

