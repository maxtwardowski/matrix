#include "Matrix.h"

Matrix::Matrix() {
	this->mtx = nullptr;
}

Matrix::Matrix(unsigned int cols, unsigned int rows) {
	try {
		rcmatrix *rcm = new rcmatrix(cols, rows);
		this->mtx = rcm;
	}
	catch(const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
}

Matrix::~Matrix() {
	this->mtx->rcount--;
	if (!this->mtx->rcount)
		delete this->mtx;
	else
		this->mtx = nullptr;
}

Matrix::Matrix(const Matrix& m) {
	this->mtx = m.mtx;
	m.mtx->rcount++;
}

void Matrix::operator=(const Matrix& m) {
	m.mtx->rcount++;
	if(--this->mtx->rcount == 0)
		delete this->mtx;
	this->mtx = m.mtx;
}

bool Matrix::operator==(const Matrix& m) const {
	if (this->getColNo() == m.getColNo() && this->getRowNo() == m.getRowNo()) {
		for (unsigned int i = 0; i < this->getRowNo(); i++)
			for (unsigned int j = 0; j < this->getColNo(); j++)
				if (this->mtx->data[i][j] != m.mtx->data[i][j])
					return false;
		return true;
	}
	return false;
}

bool Matrix::operator!=(const Matrix& m) const {
	return !(*this == m);
}

bool Matrix::areDimsEven(const Matrix& m) const {
	return (this->mtx->colno == m.mtx->colno && this->mtx->rowno == m.mtx->rowno);
}

Matrix& Matrix::operator+=(const Matrix& m) {
	try {
		if (this->areDimsEven(m)) {
			this->mtx = this->mtx->detach();
			for (unsigned int i = 0; i < this->getRowNo(); i++)
				for (unsigned int j = 0; j < this->getColNo(); j++)
					this->mtx->data[i][j] += m.mtx->data[i][j];
		} else
			throw Matrix::UnevenMatrixDimensions();
	} catch (Matrix::UnevenMatrixDimensions& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& m) {
	try {
		if (this->areDimsEven(m)) {
			this->mtx = this->mtx->detach();
			for (unsigned int i = 0; i < this->getRowNo(); i++)
				for (unsigned int j = 0; j < this->getColNo(); j++)
					this->mtx->data[i][j] -= m.mtx->data[i][j];
		} else
			throw UnevenMatrixDimensions();
	} catch (UnevenMatrixDimensions& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& m) {
	if (this->getColNo() == m.getRowNo()) {
		rcmatrix * newm;
		try {
			rcmatrix * r = new rcmatrix(this->getRowNo(), m.getColNo());
			newm = r;
		} catch (const std::bad_alloc& e) {
			std::cerr << e.what() << std::endl;
			abort();
		}
		for (unsigned int i = 0; i < this->getRowNo(); i++)
			for (unsigned int j = 0; j < m.getColNo(); j++)
				for (unsigned int k = 0; k < this->getColNo(); k++)
					newm->data[i][j] += this->mtx->data[i][k] * m.mtx->data[k][j];
		if (--this->mtx->rcount == 0)
			delete this->mtx;
		this->mtx = newm;
	}
	return *this;
}

Matrix Matrix::operator+(const Matrix& m) const {
	try {
		if (this->areDimsEven(m))
			return Matrix(*this) += m;
		else
			throw UnevenMatrixDimensions();
	} catch (UnevenMatrixDimensions& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
}

Matrix Matrix::operator-(const Matrix& m) const {
	try {
		if (this->areDimsEven(m))
			return Matrix(*this) -= m;
		else
			throw UnevenMatrixDimensions();
	} catch (UnevenMatrixDimensions& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
}

Matrix Matrix::operator*(const Matrix& m) const {
	return Matrix(*this) *= m;
}

std::istream& operator>>(std::istream& stream, Matrix& m) {
	unsigned int x, y;
	try {
		if (!(stream >> x) || !(stream >> y))
			throw Matrix::IncorrectInputDataType();
	} catch (Matrix::IncorrectInputDataType& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	Matrix::rcmatrix *rcm;
	try {
		rcm = new Matrix::rcmatrix(x, y);
	} catch(const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	for (unsigned int i = 0; i < y; i++)
		for(unsigned int j = 0; j < x; j++) {
			try {
				if (!(stream >> rcm->data[i][j]))
					throw Matrix::IncorrectInputDataType();
			} catch (Matrix::IncorrectInputDataType& e) {
				std::cerr << e.what() << std::endl;
				abort();
			}
		}
	m.mtx = rcm;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Matrix& m) {
	for (unsigned int i = 0; i < m.getRowNo(); i++) {
		for(unsigned int j = 0; j < m.getColNo(); j++)
			stream << m.mtx->data[i][j] << "\t";
		stream << std::endl;
	}
	return stream;
}

unsigned int Matrix::getColNo() const {
	return this->mtx->colno;
}

unsigned int Matrix::getRowNo() const {
	return this->mtx->rowno;
}

Matrix::rcmatrix::rcmatrix(unsigned int colno, unsigned int rowno) {
	this->colno = colno;
	this->rowno = rowno;
	this->rcount = 1;
	try {
		this->data = new double*[rowno]{};
	} catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	for (unsigned int i = 0; i < rowno; i++)
		try {
			this->data[i] = new double[colno]{};
		} catch (const std::bad_alloc& e) {
			std::cerr << e.what() << std::endl;
			abort();
		}
}

Matrix::rcmatrix::~rcmatrix() {
	for (unsigned int i = 0; i < rowno; i++)
		delete [] data[i];
	delete [] data;
}

Matrix::rcmatrix* Matrix::rcmatrix::detach() {
	if (this->rcount == 1)
		return this;
	rcmatrix *mat;
	try {
		rcmatrix *r = new rcmatrix(this->colno, this->rowno);
		mat = r;
	} catch (const std::bad_alloc& e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	for (unsigned int i = 0; i < this->rowno; i++)
		for (unsigned int j = 0; j < this->colno; j++)
			mat->data[i][j] = this->data[i][j];
	this->rcount--;
	return mat;
}

double Matrix::operator()(unsigned int col, unsigned int row) const {
	return Mref(*this, col, row);
}

Matrix::Mref Matrix::operator()(unsigned int col, unsigned int row) {
	try {
		if (this->getColNo() < col || this->getRowNo() < row) {
			throw Matrix::MatrixIndexOutOfRange();
		}
	} catch (Matrix::MatrixIndexOutOfRange &e) {
		std::cerr << e.what() << std::endl;
		abort();
	}
	return Mref(*this, col, row);
}

// reading
Matrix::Mref::operator double() const {
	return this->matrix.mtx->data[this->rowno][this->colno];
}

// writing
Matrix::Mref& Matrix::Mref::operator=(double d) {
	this->matrix.mtx->data[this->rowno][this->colno] = d;
	return *this;
}

// triggers the actual writing function
Matrix::Mref& Matrix::Mref::operator=(const Mref& ref) {
	return operator=((double)ref);
}