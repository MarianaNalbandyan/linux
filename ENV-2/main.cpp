#include <iostream>
#include <vector>
#include "Sort.h"
#include "Complex.h"

int main()
{
	vector<Complex> complexNumbers = 
	{
	Complex(1, 7),
	Complex(2, -3),
	Complex(0, 4),
	Complex(-5, 2),
	Complex(3, 3),
	};
	
	for (auto& num : complexNumbers)
	{
		std::cout << num << endl;
	}
	
	bubbleSort(complexNumbers);
	for (auto& num : complexNumbers)
	{
		std::cout << num << endl;
	}
	
	return 0;
}
