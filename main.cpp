#include <iostream>
#include <fstream>
#include "utils.h"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		std::cerr << "Didn't get path for matrix A" << std::endl;
		return 2;
	}
	if (argc == 2)
	{
		std::cerr << "Didn't get path for matrix B" << std::endl;
		return 2;
	}

	std::string filePathMatrixA(argv[1]);
	std::string filePathMatrixB(argv[2]);
	
	ParralelMultiplicator pm;

	int status = 0;

	Matrix A = pm.readMatrixFromFile(filePathMatrixA, &status);
	if (status != 0)
	{
		return status;
	}

	Matrix B = pm.readMatrixFromFile(filePathMatrixB, &status);
	if (status != 0)
	{
		return status;
	}

	Matrix result_block = pm.parralelMultBlock(A, B);

	for (auto a : result_block)
	{
		for (int b : a)
		{
			std::cout << b << '\t';
		}
		std::cout << '\n';
	}

	return 0;
}