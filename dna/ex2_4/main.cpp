#include <iostream>
#include <string>

std::string f(unsigned n) 
{
	if (n == 0)
		return "g0+g0";
	if (n == 1)
		return "g1";

	std::string a = f(n - 1);
	std::string b = f(n - 2);
	std::string c = f(n - 2);

	return "g(" + a + "+" + b + "+" + c + ")";
}

int main(int argc, char* argv[])
{
	std::cout << f(atoi(argv[1])) << std::endl;
	return 0;
}

