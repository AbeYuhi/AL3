#include "Vector3.h"

Vector3 operator+=(Vector3& num1, const Vector3& num2) {
	num1.x = num1.x + num2.x;
	num1.y = num1.y + num2.y;
	num1.z = num1.z + num2.z;

	return num1;
}

Vector3 operator-=(Vector3& num1, const Vector3& num2) {
	num1.x = num1.x - num2.x;
	num1.y = num1.y - num2.y;
	num1.z = num1.z - num2.z;

	return num1;
}

Vector3 operator-(Vector3& num1, const Vector3& num2) {
	Vector3 num3 = { 0 };
	num3.x = num1.x - num2.x;
	num3.y = num1.y - num2.y;
	num3.z = num1.z - num2.z;

	return num3;
}

Vector3 operator*=(Vector3& num1, const float num2) {
	num1.x = num1.x * num2;
	num1.y = num1.y * num2;
	num1.z = num1.z * num2;

	return num1;
}

Vector3 operator*(Vector3& num1, const float num2) {
	Vector3 num3 = { 0 };
	num3.x = num1.x * num2;
	num3.y = num1.y * num2;
	num3.z = num1.z * num2;

	return num3;
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = v1.x + v2.x;

	v3.y = v1.y + v2.y;

	v3.z = v1.z + v2.z;

	return v3;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = v1.x - v2.x;

	v3.y = v1.y - v2.y;

	v3.z = v1.z - v2.z;

	return v3;
}

Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 v3 = { 0, 0, 0 };

	v3.x = scalar * v.x;

	v3.y = scalar * v.y;

	v3.z = scalar * v.z;

	return v3;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float dot = 0.0f;

	dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return dot;
}

float Length(const Vector3& v) {
	float length = 0;

	length = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return length;
}

Vector3 Normalize(const Vector3& v1) {
	Vector3 v2 = { 0, 0, 0 };
	float length = Length(v1);

	v2.x = v1.x / length;
	v2.y = v1.y / length;
	v2.z = v1.z / length;

	return v2;
}