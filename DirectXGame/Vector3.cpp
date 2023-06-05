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

float Length(const Vector3& v1, const Vector3& v2) {
	float length = 0;

	length = sqrtf(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2) + powf(v1.z - v2.z, 2));

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

Vector3 CatmullRomSpline(std::vector<Vector3> controlPoints, float t) {
	Vector3 vector;
	//controlePointsの要素数
	auto controlPointsNum = controlPoints.size();
	//tがどこの補間を進んでるかを求める
	auto movedRate = 1.0f / controlPointsNum;

	auto moveRate = 0;
	
	for (int i = 0; i < controlPointsNum - 1; i++) {
		auto rate = i * movedRate;

		if (rate <= t && t < rate + movedRate) {
			moveRate = i;
			break;
		}
	}

	float movedT = t * (controlPointsNum);
	movedT -= int(movedT);

	if (moveRate == 0) {
		vector = CatmullRom(controlPoints[0], controlPoints[0], controlPoints[1], controlPoints[2], movedT);
	} else if (moveRate == controlPointsNum - 2) {
		vector = CatmullRom(controlPoints[controlPointsNum - 3], controlPoints[controlPointsNum - 2], controlPoints[controlPointsNum - 1], controlPoints[controlPointsNum - 1], movedT);
	}
	else {
		vector = CatmullRom(controlPoints[moveRate - 1], controlPoints[moveRate], controlPoints[moveRate + 1], controlPoints[moveRate + 2], movedT);
	}

	return vector;
}

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	Vector3 p;

	p.x = 1.0f / 2.0f * (((-1.0f * p0.x) + 3.0f * p1.x - (3.0f * p2.x) + p3.x) * powf(t, 3) + (2.0f * p0.x - (5.0f * p1.x) + 4.0f * p2.x - p3.x) * powf(t, 2) + (-1.0f * p0.x + p2.x) * t + 2.0f * p1.x);
	p.y = 1.0f / 2.0f * (((-1.0f * p0.y) + 3.0f * p1.y - (3.0f * p2.y) + p3.y) * powf(t, 3) + (2.0f * p0.y - (5.0f * p1.y) + 4.0f * p2.y - p3.y) * powf(t, 2) + (-1.0f * p0.y + p2.y) * t + 2.0f * p1.y);
	p.z = 1.0f / 2.0f * (((-1.0f * p0.z) + 3.0f * p1.z - (3.0f * p2.z) + p3.z) * powf(t, 3) + (2.0f * p0.z - (5.0f * p1.z) + 4.0f * p2.z - p3.z) * powf(t, 2) + (-1.0f * p0.z + p2.z) * t + 2.0f * p1.z);

	return p;
}