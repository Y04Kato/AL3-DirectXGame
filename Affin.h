#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// 1 X����]�s��
Matrix4x4 MakeRotateXmatrix(float radian);

// 2 Y����]�s��
Matrix4x4 MakeRotateYmatrix(float radian);

// 3 Z����]�s��
Matrix4x4 MakeRotateZmatrix(float radian);

// ���s�ړ�
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// �g��k��
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// �A�t�B���ϊ�
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);