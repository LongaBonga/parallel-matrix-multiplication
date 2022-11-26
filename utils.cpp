#include <iostream>
#include <string>
#include <pthread.h>
#include "utils.h"

Matrix ParralelMultiplicator::readMatrixFromFile(std::string& filePath, int *status)
{
	int rows = 0;
	int cols = 0;

	std::ifstream in(filePath);
	if (!in.is_open())
	{
		std::cerr << "Couldn't open a file of Matrix: " << filePath << std::endl;
		*status = 3;
		return Matrix();
	}

	in >> rows >> cols;

	Matrix mat;

	for (size_t i = 0; i < rows; ++i)
	{
		mat.push_back(std::vector<int>(cols));
		for (size_t j = 0; j < cols; ++j)
		{
			if (in.eof())
			{
				std::cerr << "Wrong file shape: " << std::endl;
				*status = 4;
				return Matrix();
			}
			in >> mat[i][j];
		}
	}

	return mat;
}

Matrix ParralelMultiplicator::parralelMultRow(Matrix& A, Matrix& B)
{
	if (!checkDimensions(A, B))
	{
		return Matrix();
	}

	size_t rowsA = A.size();
	size_t colsA = A[0].size();
	size_t rowsB = B.size();
	size_t colsB = B[0].size();

	Matrix result = Matrix(rowsA, std::vector<int>(colsB));

	//

	return Matrix();
}

Matrix ParralelMultiplicator::parralelMultColumn(Matrix& A, Matrix& B)
{
	if (!checkDimensions(A, B))
	{
		return Matrix();
	}

	size_t rowsA = A.size();
	size_t colsA = A[0].size();
	size_t rowsB = B.size();
	size_t colsB = B[0].size();

	Matrix result = Matrix(rowsA, std::vector<int>(colsB));

	//

	return Matrix();
}

Matrix ParralelMultiplicator::parralelMultBlock(Matrix& A, Matrix& B)
{
	if (!checkDimensions(A, B))
	{
		return Matrix();
	}

	size_t rowsA = A.size();
	size_t colsA = A[0].size();
	size_t rowsB = B.size();
	size_t colsB = B[0].size();

	Matrix result = Matrix(rowsA, std::vector<int>(colsB));

	

	return Matrix();
}

bool ParralelMultiplicator::checkDimensions(Matrix& A, Matrix& B)
{
	bool res = A.size() && B.size();
	if (res)
	{
		res = A[0].size() == B.size();
	}
	
	if (!res)
	{
		std::cerr << "Matrices can not be multiplied!" << std::endl;
	}

	return res;
}
