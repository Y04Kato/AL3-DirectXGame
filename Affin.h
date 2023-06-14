#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

//Vector3���m�̑����Z
Vector3 VectorMultiply(const Vector3& translation, const Vector3& move);

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

//TransformNormal
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// �A�t�B���ϊ�
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);