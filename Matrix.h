#pragma once
#include <iostream>

class Matrix {
	class Mref;
	struct rcmatrix;
	struct UnevenMatrixDimensions;
	struct IncorrectInputDataType;
	struct MatrixIndexOutOfRange;
public:
	rcmatrix *mtx;
	Matrix();
	Matrix(unsigned int x, unsigned int y);
	Matrix(const Matrix& m);
	~Matrix();
	unsigned int getColNo() const;
	unsigned int getRowNo() const;
	void operator=(const Matrix& m);
	bool operator==(const Matrix& m) const;
	bool operator!=(const Matrix& m) const;
	bool areDimsEven(const Matrix &m) const;
	Mref operator()(unsigned int x, unsigned int y);
	double operator()(unsigned int x, unsigned int y) const;
	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);
	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix operator*(const Matrix& m) const;
	friend std::istream& operator>>(std::istream& stream, Matrix& m);
	friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);
};

struct Matrix::rcmatrix {
	double **data;
	unsigned int rcount;
	unsigned int colno, rowno;
	rcmatrix(unsigned int colno = 0, unsigned int rowno = 0);
	~rcmatrix();
	rcmatrix* detach();
};

class Matrix::Mref {
	friend class Matrix;
	const Matrix& matrix;
	unsigned int colno, rowno;
	Mref(const Matrix& mtx, unsigned int col, unsigned int row)
		:matrix(mtx), colno(col), rowno(row) {};
public:
	operator double() const;
	Matrix::Mref& operator=(double d);
	Matrix::Mref& operator=(const Mref& ref);
};

struct Matrix::UnevenMatrixDimensions : public std::exception {
	const char* what () const throw () {
		return "The compared matrices have different dimensions";
	}
};

struct Matrix::IncorrectInputDataType : public std::exception {
	const char* what () const throw () {
		return "Wrong type of the input data";
	}
};

struct Matrix::MatrixIndexOutOfRange : public std::exception {
	const char* what () const throw () {
		return "Index refers to a non-existant matrix element";
	}
};