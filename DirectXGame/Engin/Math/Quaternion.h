/**
 * @file Quaternion.h
 * @brief Quaternionクラス
 * @author Yamashita Tubasa
 */

#pragma once

#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float x, float y, float z, float w);

	// Quaternionの積
	Quaternion Multiply(Quaternion q);
	// 単位Quaternionを返す
	Quaternion IdentityQuaternion();
	// 共役Quaternionを返す
	Quaternion Conjugate();
	// Quaternionのnormを返す
	float Norm() const;
	// 正規化したQuaternionを返す
	Quaternion Normalize();
	// 逆Quaternionを返す
	Quaternion Inverse();
	// ベクトルをQuaternionで回転させた結果のベクトルを求める
	Vector3 RotateVector(const Vector3& v);
	// Quaternionから回転行列を求める
	Matrix4 MakeRotateMatrix();

	// 単項演算子オーバーロード
	Quaternion operator+() const;
	Quaternion operator-() const;

	// 代入演算子オーバーロード
	Quaternion& operator+=(const Quaternion& q);
	Quaternion& operator-=(const Quaternion& q);
	Quaternion& operator*=(float s);
	Quaternion& operator/=(float s);
};

// 2項演算子オーバーロード
// いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備している
const Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
const Quaternion operator*(const Quaternion& q, float s);
const Quaternion operator*(float s, const Quaternion& q);
const Quaternion operator/(const Quaternion& q, float s);

// 任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& axis, float angle);

// 球面線形補間
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

// uからvへの回転を生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

