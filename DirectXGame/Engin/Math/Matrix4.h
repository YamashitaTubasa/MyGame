#pragma once

#include "Vector3.h"

#define PI 3.14159265359

// ========//
// Matrix4 //
// ========//
class Matrix4
{
public:
	float m[4][4];

public:
	Matrix4();
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 単位行列を求める
	static Matrix4 Identity();

	// 拡大縮小行列の設定
	Matrix4 Scale(const Vector3& s);

	// 回転行列の設定
	Matrix4 RotateX(float angle);
	Matrix4 RotateY(float angle);
	Matrix4 RotateZ(float angle);

	// ラジアンに変換
	static float ConvertToRadians(float rotation);

	// 平行移動行列の作成
	Matrix4 Translate(const Vector3& t);

	// 座標変換（ベクトルと行列の掛け算をする）
	Vector3 Transform(const Vector3& v, const Matrix4& m);

	Matrix4 operator*(const Matrix4& m1) const;

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m1, const Matrix4& m2);
};

// 2項演算子オーバーロード
const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
const Vector3 operator*(const Vector3& v, const Matrix4& m);