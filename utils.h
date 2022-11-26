#pragma once

#ifndef PARALLEL_MULTIPLICATOR
#define PARALLEL_MULTIPLICATOR

#include <vector>
#include <fstream>

using Matrix = std::vector<std::vector<int>>;

class ParralelMultiplicator
{
public:
	Matrix readMatrixFromFile(std::string& filePath, int *status);

	Matrix parralelMultRow(Matrix& A, Matrix& B);
	Matrix parralelMultColumn(Matrix& A, Matrix& B);
	Matrix parralelMultBlock(Matrix& A, Matrix& B);
	bool checkDimensions(Matrix& A, Matrix& B);

//private:
//	Matrix A;
//	Matrix B;

};

#endif // !PARALLEL_MULTIPLICATOR
