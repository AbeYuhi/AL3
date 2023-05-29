#pragma once
#include <math.h>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

//算術演算子のオーバーロード
Vector3 operator+(Vector3& num1, const Vector3& num2);
Vector3 operator-(Vector3& num1, const Vector3& num2);
Vector3 operator*(Vector3& num1, const float num2);
Vector3 operator+=(Vector3& num1, const Vector3& num2);
Vector3 operator-=(Vector3& num1, const Vector3& num2);
Vector3 operator*=(Vector3& num1, const float num2);

//関数

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

float Length(const Vector3& v1, const Vector3& v2);

Vector3 Normalize(const Vector3& v1);