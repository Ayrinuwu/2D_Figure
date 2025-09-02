#pragma once

#include "Matrix.h"


class Model2D {
private:
    Matrix<double> vertices;
    Matrix<int> adjacencyMatrix;
    Matrix<double> accumulatedTransform;

public:
    Model2D()
        : vertices(Matrix<double>()), adjacencyMatrix(Matrix<int>())
    {
        double startTransform[9] = { 1,0,0,
                                     0,1,0,
                                     0,0,1 };
        accumulatedTransform = Matrix<double>(3, 3, startTransform);

    }
    Model2D(const Matrix<double>& vertices, const Matrix<int>& adjacencyMatrix) 
        : vertices(vertices), adjacencyMatrix(adjacencyMatrix) 
    {
        double startTransform[9] = { 1,0,0,
                                     0,1,0,
                                     0,0,1 };
        accumulatedTransform = Matrix<double>(3, 3, startTransform);

    }

    void Apply(const Matrix<double>& transform) {
        accumulatedTransform = transform * accumulatedTransform;
        vertices = transform * vertices;
    }

    void printVertices() const {
        for (int i = 0; i < vertices.getRows(); ++i) {
            for (int j = 0; j < vertices.getCols(); ++j) {
                std::cout << vertices.get(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    int getNumVertices() const { return vertices.getCols(); }
    int getAdjacencyMatrix(int i, int j) const { return adjacencyMatrix.get(i,j); }
    double getVerticeX(int i) const { return vertices.get(0, i)/ vertices.get(2, i); }
    double getVerticeY(int i) const { return vertices.get(1, i) / vertices.get(2, i); }
};