#pragma once
#include <math.h>

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;
};

//算術演算子のオーバーロード
Vector2 operator+=(Vector2& num1, const Vector2& num2);
Vector2 operator-=(Vector2& num1, const Vector2& num2);
Vector2 operator-(Vector2& num1, const Vector2& num2);
Vector2 operator*=(Vector2& num1, const float num2);
Vector2 operator*(Vector2& num1, const float num2);

float Length(const Vector2& v);
