#include "Vector2.h"

Vector2 operator+=(Vector2& num1, const Vector2& num2) {
	num1.x = num1.x + num2.x;
	num1.y = num1.y + num2.y;

	return num1;
}

Vector2 operator-=(Vector2& num1, const Vector2& num2) {
	num1.x = num1.x - num2.x;
	num1.y = num1.y - num2.y;

	return num1;
}

Vector2 operator-(Vector2& num1, const Vector2& num2) {
	Vector2 num3 = { 0 };
	num3.x = num1.x - num2.x;
	num3.y = num1.y - num2.y;

	return num3;
}

Vector2 operator*=(Vector2& num1, const float num2) {
	num1.x = num1.x * num2;
	num1.y = num1.y * num2;

	return num1;
}

Vector2 operator*(Vector2& num1, const float num2) {
	Vector2 num3 = { 0 };
	num3.x = num1.x * num2;
	num3.y = num1.y * num2;

	return num3;
}

float Length(const Vector2& v) {
	float length = 0;

	length = sqrtf(powf(v.x, 2) + powf(v.y, 2));

	return length;
}