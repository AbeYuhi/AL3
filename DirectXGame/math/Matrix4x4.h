#pragma once
#include <Vector3.h>
#include <cassert>
#include <cmath>

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 {
	float m[4][4];
};

//算術演算子のオーバーロード
Matrix4x4 operator+(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 operator-(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 operator*(const Matrix4x4& matrix1, const float num2);
Matrix4x4 operator+=(Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 operator-=(Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 operator*=(Matrix4x4& matrix1, const float num2);
Matrix4x4 operator*=(Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 operator*(Matrix4x4& matrix1, const Matrix4x4& matrix2);


Matrix4x4 Add(Matrix4x4 matrix1, Matrix4x4 matrix2);

Matrix4x4 Subtract(Matrix4x4 matrix1, Matrix4x4 matrix2);

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2);

float Det(Matrix4x4 matrix);

Matrix4x4 Inverse(Matrix4x4 matrix);

Matrix4x4 Transpose(Matrix4x4 matrix);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateXYZMatrix(const Matrix4x4& matrixX, const Matrix4x4& matrixY, const Matrix4x4& matrixZ);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 CalculateWorldMatrix();

Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);