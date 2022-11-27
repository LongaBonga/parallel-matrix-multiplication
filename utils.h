#pragma once

#ifndef PARALLEL_MULTIPLICATOR
#define PARALLEL_MULTIPLICATOR

#include <vector>
#include <fstream>
#include <string>

using Matrix = std::vector<std::vector<int> >;

class ParralelMultiplicator {
public:
    Matrix readMatrixFromFile(const std::string& filePath, int *status);

    Matrix parralelMultRow(const Matrix& A, const Matrix& B);
    Matrix parralelMultColumn(const Matrix& A, const Matrix& B);
    Matrix parralelMultBlock(const Matrix& A, const Matrix& B);
    bool checkDimensions(const Matrix& A, const Matrix& B);
};
#endif  // !PARALLEL_MULTIPLICATOR
