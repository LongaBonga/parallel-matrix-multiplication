#define HAVE_STRUCT_TIMESPEC
#include <iostream>
#include <string>
#include <pthread.h>
#include <cmath>
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

struct MatrixPairForBlockMult
{
	Matrix& A;
	Matrix& B;
	Matrix& C;
	
	long thread_id;
	size_t grid_size;
	size_t block_size;
};

pthread_mutex_t mutex_for_block_mult;
pthread_mutex_t mutex_check;

void* parralelMultBlockProcedure(void* mat_)
{
	MatrixPairForBlockMult* matrix_pair = reinterpret_cast<MatrixPairForBlockMult*>(mat_);
	Matrix &AMatrix = matrix_pair->A;
	Matrix &BMatrix = matrix_pair->B;
	Matrix &CMatrix = matrix_pair->C;
	size_t GridSize = matrix_pair->grid_size;
	size_t BlockSize = matrix_pair->block_size;
	size_t ThreadID = matrix_pair->thread_id;
	size_t Size = AMatrix.size();

	int RowIndex = ThreadID / GridSize;
	int ColIndex = ThreadID % GridSize;

	//std::cerr << "GridSize:" << GridSize << std::endl;
	//std::cerr << "BlockSize:" << BlockSize << std::endl;
	//std::cerr << "ThreadID:" << ThreadID << std::endl;
	//std::cerr << "Size:" << Size << std::endl;
	//std::cerr << "RowIndex:" << RowIndex << std::endl;
	//std::cerr << "ColIndex:" << ColIndex << std::endl;
	//for (int iter = 0; iter < GridSize; iter++) {
	//	for (int i = RowIndex * BlockSize;
	//		i < (RowIndex + 1) * BlockSize; i++)
	//		for (int j = ColIndex * BlockSize;
	//			j < (ColIndex + 1) * BlockSize; j++)
	//			for (int k = iter * BlockSize;
	//				k < (iter + 1) * BlockSize; k++)
	//				CMatrix[i * Size + j] +=
	//				AMatrix[i * Size + k] * BMatrix[k * Size + j];


	for (int iter = 0; iter < GridSize; iter++)
	{
		for (int i = RowIndex * BlockSize;
			i < (RowIndex + 1) * BlockSize && i < Size; i++)
		{
			for (int j = ColIndex * BlockSize;
				j < (ColIndex + 1) * BlockSize && j < Size; j++)
			{
				for (int k = iter * BlockSize;
					k < (iter + 1) * BlockSize && k < Size; k++)
				{
					pthread_mutex_lock(&mutex_for_block_mult);
					CMatrix[i][j] += AMatrix[i][k] * BMatrix[k][j];
					pthread_mutex_unlock(&mutex_for_block_mult);
				}
			}
		}
	}

	return matrix_pair;
}

unsigned findNearestPowOfThreadSize(unsigned size)
{
	int sqrt_size = static_cast<int>(std::sqrt(size));
	return  sqrt_size * sqrt_size;
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

	unsigned int thread_count = 10;

	if (thread_count > result.size())
	{
		thread_count = result.size();
	}

	thread_count = findNearestPowOfThreadSize(thread_count);

	std::vector<pthread_t> threads(thread_count);
	std::vector<MatrixPairForBlockMult*> structuresForThreads(thread_count);

	size_t GridSize = static_cast<unsigned>(std::sqrt(thread_count));

	pthread_mutex_init(&mutex_for_block_mult, NULL);

	for (long thread = 0; thread < thread_count; ++thread)
	{
		structuresForThreads[thread] = new MatrixPairForBlockMult{ A, B, result,
			thread,
			GridSize,
			result.size() / GridSize };

		pthread_create(&threads[thread], NULL, parralelMultBlockProcedure, reinterpret_cast<void*>(structuresForThreads[thread]));
	}


	for (long thread = 0; thread < thread_count; ++thread)
	{
		pthread_join(threads[thread], NULL);
		delete structuresForThreads[thread];
	}

	pthread_mutex_destroy(&mutex_for_block_mult);

	return result;
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
