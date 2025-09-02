#include <iostream>
#include <cmath>
template <typename T>
class Matrix {
protected:
    T** data;
    int rows;
    int cols;

public:
    Matrix() : data(nullptr), rows(0), cols(0) {}

   
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        
        data = new T * [rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new T[cols](); 
        }
    }

    Matrix(int rows, int cols, T*list) : rows(rows), cols(cols) {

        data = new T * [rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new T[cols];
            for (int j = 0; j < cols; ++j)
            {
                data[i][j] = list[i * cols + j];
            }
        }
    }
    
    // Конструктор копирования
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new T * [rows];
        for (int i = 0; i < rows; ++i) {
            data[i] = new T[cols];
            for (int j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deleteMatrix();
            rows = other.rows;
            cols = other.cols;
            data = new T * [rows];
            for (int i = 0; i < rows; ++i) {
                data[i] = new T[cols];
                for (int j = 0; j < cols; ++j) {
                    data[i][j] = other.data[i][j];
                }
            }
        }
        return *this;
    }

    ~Matrix() {
        deleteMatrix();
    }

    void deleteMatrix() {
        if (data != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
            data = nullptr;
        }
    }

    T get(int row, int col) const {
        return data[row][col];
    }

    T& operator()(int row, int col) {
        return data[row][col];
    }

    Matrix operator*(const Matrix& other) const {
        
        Matrix result(this->rows, other.cols);
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < this->cols; ++k) {
                    result(i, j) += this->data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator+(const Matrix& other) const {
        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->cols; ++j) {
                
                result(i, j) = this->data[i][j] + other.data[i][j];
                
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {      
        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->cols; ++j) {

                result(i, j) = this->data[i][j] - other.data[i][j];

            }
        }
        return result;
    }


    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

Matrix<double> translate(double dx, double dy) {
    double T[9] = { 1., 0, dx,
                    0 , 1.,dy,
                    0 , 0 ,1. };
    return Matrix<double>(3, 3, T);
}

Matrix<double> scale(double sx, double sy) {
    double S[9] = { sx, 0, 0,
                    0 , sy,0,
                    0 , 0 ,1. };
    return Matrix<double>(3, 3, S);
}


Matrix<double> rotate(double angle) {
    double rad = angle * 3.14 / 180.0;
    double R[9] = { cos(rad), -sin(rad), 0,
                    sin(rad), cos(rad) , 0,
                    0       , 0        , 1. };
    return Matrix<double>(3, 3, R);
}

Matrix<double> translation(double dx, double dy, double dz) {
    double T[16] = { 1, 0, 0, dx,
                   0, 1, 0, dy,
                   0, 0, 1, dz,
                   0, 0, 0, 1 };
    return Matrix<double>(4, 4, T);
}

Matrix<double> scaling(double sx, double sy, double sz) {
    double S[16] = { sx, 0, 0,
                    0 , sy,0,
                    0, 0, sz, 0,
                    0 , 0 ,1. };
    return Matrix<double>(4, 4, S);
}

Matrix<double> rotation_x(double angle) {
    double rad = angle * 3.14 / 180.0;
    double R[16] = { 1, 0,        0,         0,
                     0, cos(rad), -sin(rad), 0,
                     0, sin(rad), cos(rad) , 0,
                     0, 0,        0,         1. };
    return Matrix<double>(4, 4, R);
}

Matrix<double> rotation_y(double angle) {
    double rad = angle * 3.14 / 180.0;
    double R[16] = { cos(rad), 0, sin(rad), 0,
                     0,        1,  0,       0,
                    -sin(rad), 0, cos(rad), 0,
                     0,         0, 0,       1. };
    return Matrix<double>(4, 4, R);
}
Matrix<double> rotation_y(double angle) {
    double rad = angle * 3.14 / 180.0;
    double R[16] = { cos(rad), 0, sin(rad), 0,
                     0,        1,  0,       0,
                    -sin(rad), 0, cos(rad), 0,
                     0,         0, 0,       1. };
    return Matrix<double>(4, 4, R);
}

Matrix<double> rotation_z(double angle) {
    double rad = angle * 3.14 / 180.0;
    double R[16] = { cos(rad), -sin(rad), 0, 0,
                     sin(rad), cos(rad),  0, 0,
                     0,        0,         1, 0,
                     0,        0,         0, 1. };
    return Matrix<double>(4, 4, R);
}