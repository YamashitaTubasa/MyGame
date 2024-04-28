/**
 * @file Collision.cpp
 * @brief 当たり判定に関するクラス
 * @author Yamashita Tubasa
 */

#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter, [[maybe_unused]] DirectX::XMVECTOR* r)
{
	// 座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphereA.center_, sphereB.center_);
	// 距離の絶対値が半径より大きければ当っていない
	if (sphereB.radius_ > sphereA.radius_) return false;

	// 擬似交点を計算
	if (inter) {
		// 球上の最近接点を、擬似交点とする
		*inter = sphereB.center_ + sphereA.center_;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
    // 座標系の原点から球の中心座標への距離
    XMVECTOR distV = XMVector3Dot(sphere.center_, plane.normal_);
    // 平面の原点距離を減算することで、平面と球の中心との距離が出る
    float dist = distV.m128_f32[0] - plane.distance_;
    // 距離の絶対値が半径より大きければ当っていない
    if (fabsf(dist) > sphere.radius_) return false;

    // 擬似交点を計算
    if (inter) {
        // 平面上の最近接点を、擬似交点とする
        *inter = -dist * plane.normal_ + sphere.center_;
    }

    return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1_ - triangle.p0_;
	XMVECTOR p0_p2 = triangle.p2_ - triangle.p0_;
	XMVECTOR p0_pt = point - triangle.p0_;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0_;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1_;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1_;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2_;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2_;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center_, triangle, &p);
	// 点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center_;
	// 距離の二乗を求める
	// （同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	v = XMVector3Dot(v, v);
	// 球と三角形の距離が半径以下なら当っていない
	if (v.m128_f32[0] > sphere.radius_ * sphere.radius_) return false;
	// 擬似交点を計算
	if (inter) {
		// 三角形上の最近接点pを擬似交点とする
		*inter = p;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面法線のレイの方向ベクトルの内積
	float d1 = XMVector3Dot(plane.normal_, ray.dir_).m128_f32[0];
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 視点と原点の距離（平面の法線方向）
	// 面法線とレイの始点座標（位置ベクトル）の内積
	float d2 = XMVector3Dot(plane.normal_, ray.start_).m128_f32[0];
	// 始点と平面の距離（平面の法線方向）
	float dist = d2 - plane.distance_;
	// 始点と平面の距離（レイ方向）
	float t = dist / -d1;
	// 交点が始点より後ろにあるので、当たらない
	if (t < 0) return false;
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	if (inter) { *inter = ray.start_ + t * ray.dir_; }

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	// 三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal_ = triangle.normal_;
	plane.distance_ = XMVector3Dot(triangle.normal_, triangle.p0_).m128_f32[0];
	// レイと平面が当たっていなければ、当っていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	// レイと平面が当たっていたので、距離と交点が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	XMVECTOR m;
	// 辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0_ - interPlane;
	XMVECTOR p0_p1 = triangle.p1_ - triangle.p0_;
	m = XMVector3Cross(pt_p0, p0_p1);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	// 辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1_ - interPlane;
	XMVECTOR p1_p2 = triangle.p2_ - triangle.p1_;
	m = XMVector3Cross(pt_p1, p1_p2);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	// 辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2_ - interPlane;
	XMVECTOR p2_p0 = triangle.p0_ - triangle.p2_;
	m = XMVector3Cross(pt_p2, p2_p0);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal_).m128_f32[0] < -epsilon) { return false; }

	// 内側なので、当たっている
	if (inter) {
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start_ - sphere.center_;
	float b = XMVector3Dot(m, ray.dir_).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius_ * sphere.radius_;
	// layの始点がshpereの外側にあり(c > 0),layがsphereから離れていく方向を指している場合(b > 0), 当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	// 負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f) { return false; }

	// レイは球と交差してる。
	// 交差する最小の値を計算
	float t = -b - sqrtf(discr);
	// tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0)t = 0.0f;
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start_ + t * ray.dir_; }

	return true;
}
