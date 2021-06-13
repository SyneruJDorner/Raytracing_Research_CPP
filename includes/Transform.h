#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
typedef double **Matrix;


class Quaternion
{
public:
	double x, y, z, w;

	Quaternion();
	bool operator==(const Quaternion &other);
	void rotateAroundX();
	void rotateAroundY();
	void rotateAroundZ();

    /*
	***************DEFAULT IDENTITY**************************************************
    ---------------------------------------------------------------------------------
    |         1         |         0         |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         1         |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         1         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------


	***************TRANSFORM MATRIX**************************************************
    ---------------------------------------------------------------------------------
    |         1         |         0         |         0         |   Translation.X   |
    ---------------------------------------------------------------------------------
    |         0         |         1         |         0         |   Translation.Y   |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         1         |   Translation.Z   |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------


	***************SCALE MATRIX******************************************************
    ---------------------------------------------------------------------------------
    |      Scale.X      |         0         |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |      Scale.Y      |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |      Scale.Z      |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------


	***************ROTATE AROUND X AXIS**********************************************
    ---------------------------------------------------------------------------------
    |         1         |         0         |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |       cos(θ)      |      -sin(θ)      |         0         |
    ---------------------------------------------------------------------------------
    |         0         |       sin(θ)      |       cos(θ)      |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------


	***************ROTATE AROUND Y AXIS**********************************************
    ---------------------------------------------------------------------------------
    |       cos(θ)      |         0         |       sin(θ)      |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         1         |         0         |         0         |
    ---------------------------------------------------------------------------------
    |      -sin(θ)      |         0         |       cos(θ)      |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------


	***************ROTATE AROUND Z AXIS**********************************************
    ---------------------------------------------------------------------------------
    |       cos(θ)      |      -sin(θ)      |         0         |         0         |
    ---------------------------------------------------------------------------------
    |       sin(θ)      |       cos(θ)      |         0         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         1         |         0         |
    ---------------------------------------------------------------------------------
    |         0         |         0         |         0         |         1         |
    ---------------------------------------------------------------------------------
    */
};

class Transform
{
public:
    Vector3 position;
    Vector3 lookAt;
    Vector3 rotation;
    Vector3 scale;
    Matrix worldMatrix;
    Matrix localMatrix;

    Transform();

    Matrix setMatrixIdentity();
    Matrix rotateAroundX(double radians);
    Matrix rotateAroundY(double radians);
    Matrix rotateAroundZ(double radians);

    Matrix matrixMultiply(Matrix matrixA, Matrix matrixB);

    void getCofactor(Matrix matrixA, Matrix matrixB, int p, int q, int n);
    int determinant(Matrix matrixA, int n); //Recursive function for finding determinant of matrix. n is current dimension of A[][].
    void adjoint(Matrix matrixA, Matrix matrixB); // Function to get adjoint of A[N][N] in adj[N][N].
    bool inverseMatrix(Matrix matrixA, Matrix matrixB);

    void updateTransform();

    Vector3	transformRotation(Matrix m, Vector3 direction);
};

Transform::Transform()
{
    this->updateTransform();
}

//Matrix Identity
Matrix Transform::setMatrixIdentity()
{
    Matrix matrix = (Matrix)malloc(sizeof(double*) * 4);
    int counter = 5;
    
    for (int x = 0; x < 4; x++)
    {
        matrix[x] = (double*) malloc(sizeof(double) * 4);
        for (int y = 0; y < 4; y++)
        {
            if (counter == 5)
            {
                matrix[x][y] = 1;
                counter--;
                continue;
            }

            matrix[x][y] = 0;
            counter--;

            if (counter == 0)
                counter = 5;
        }
    }
    return matrix;
}

//Matrix Rotation
Matrix Transform::rotateAroundX(double radians)
{
	Matrix matrix = this->setMatrixIdentity();
	matrix[1][1] = cos(radians);
	matrix[1][2] = -sin(radians);
	matrix[2][1] = sin(radians);
	matrix[2][2] = cos(radians);
	return matrix;
}

Matrix Transform::rotateAroundY(double radians)
{
	Matrix matrix = this->setMatrixIdentity();
	matrix[0][0] = cos(radians);
	matrix[0][2] = sin(radians);
	matrix[2][0] = -sin(radians);
	matrix[2][2] = cos(radians);
	return matrix;
}

Matrix Transform::rotateAroundZ(double radians)
{
	Matrix matrix = this->setMatrixIdentity();
	matrix[0][0] = cos(radians);
	matrix[0][1] = -sin(radians);
	matrix[1][0] = sin(radians);
	matrix[1][1] = cos(radians);
	return matrix;
}

//Matrix Multiplication
Matrix Transform::matrixMultiply(Matrix matrixA, Matrix matrixB)
{
    Matrix matrix = this->setMatrixIdentity();
    int x, y, k;
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            matrix[x][y] = 0;
            for (k = 0; k < 4; k++)
                matrix[x][y] += matrixA[x][k] * matrixB[k][y];
        }
    }

    return matrix;
}

//Matrix Inverse
void Transform::getCofactor(Matrix matrixA, Matrix matrixB, int p, int q, int n)
{
    int x = 0, y = 0;
  
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into matrixB only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                matrixB[x][y++] = matrixA[row][col];
  
                // Row is filled, so increase row index and
                // reset col index
                if (y == n - 1)
                {
                    y = 0;
                    x++;
                }
            }
        }
    }
}

int Transform::determinant(Matrix matrixA, int n)
{
    int D = 0; // Initialize result
  
    //  Base case : if matrix contains single element
    if (n == 1)
        return matrixA[0][0];
  
    Matrix temp = this->setMatrixIdentity(); // To store cofactors
    int sign = 1;  // To store sign multiplier
  
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(matrixA, temp, 0, f, n);
        D += sign * matrixA[0][f] * determinant(temp, n - 1);
  
        // terms are to be added with alternate sign
        sign = -sign;
    }
  
    return D;
}

void Transform::adjoint(Matrix matrixA, Matrix matrixB)
{
    if (4 == 1)
    {
        matrixB[0][0] = 1;
        return;
    }
  
    // temp is used to store cofactors of A[][]
    int sign = 1;
    Matrix temp = this->setMatrixIdentity();
  
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            // Get cofactor of A[i][j]
            getCofactor(matrixA, temp, x, y, 4);
  
            // sign of matrixB[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((x + y) % 2 == 0) ? 1 : -1;
  
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            matrixB[x][y] = (sign)*(determinant(temp, 4 - 1));
        }
    }
}

bool Transform::inverseMatrix(Matrix matrixA, Matrix matrixB)
{
    // Find determinant of A[][]
    int det = this->determinant(matrixA, 4);
    if (det == 0)
    {
        std::cout << "Singular matrix, can't find its inverse";
        return false;
    }
  
    // Find adjoint
    Matrix adj = this->setMatrixIdentity();
    adjoint(matrixA, adj);
  
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            matrixB[x][y] = adj[x][y] / float(det);
  
    return true;
}

void Transform::updateTransform()
{
    Matrix matrix;
    matrix = this->worldMatrix = this->localMatrix = this->setMatrixIdentity();

    matrix = rotateAroundX(this->rotation.x());
    this->worldMatrix = matrixMultiply(this->worldMatrix, matrix);

    matrix = rotateAroundY(this->rotation.y());
    this->worldMatrix = matrixMultiply(this->worldMatrix, matrix);

    matrix = rotateAroundZ(this->rotation.z());
    this->worldMatrix = matrixMultiply(this->worldMatrix, matrix);

    matrix = this->setMatrixIdentity();
    matrix[0][3] = this->position.x();
    matrix[1][3] = this->position.y();
    matrix[2][3] = this->position.z();
    this->worldMatrix = matrixMultiply(this->worldMatrix, matrix);

    inverseMatrix(this->worldMatrix, this->localMatrix);
}

//Transform Rotation
Vector3	Transform::transformRotation(Matrix m, Vector3 direction)
{
    Vector3	a;

    a = Vector3(0, 0, 0);
    a.e[0] = direction.x() * m[0][0] + direction.y() * m[1][0] + direction.z() * m[2][0];
    a.e[1] = direction.x() * m[0][1] + direction.y() * m[1][1] + direction.z() * m[2][1];
    a.e[2] = direction.x() * m[0][2] + direction.y() * m[1][2] + direction.z() * m[2][2];
    return a;
}
#endif