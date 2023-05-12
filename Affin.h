#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// 1 X²‰ñ“]s—ñ
Matrix4x4 MakeRotateXmatrix(float radian);

// 2 Y²‰ñ“]s—ñ
Matrix4x4 MakeRotateYmatrix(float radian);

// 3 Z²‰ñ“]s—ñ
Matrix4x4 MakeRotateZmatrix(float radian);

// •½sˆÚ“®
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// Šg‘åk¬
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// ƒAƒtƒBƒ“•ÏŠ·
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);