/**
 * @file EasingManager.h
 * @brief イージングの管理クラス
 * @author Yamashita Tubasa
 */

#pragma once

class EasingManager
{
	/// <summary>
	/// イージング
	/// </summary>
	/// <param name="t"> 時間 </param>
	/// <param name="b"> 開始位置 </param>
	/// <param name="c"> 開始位置 - 終了位置の差 </param>
	/// <param name="d"> 合計時間 </param>
	/// <returns></returns>
	
public: // 中間地点が一番遅い挙動(下にいくほど挙動の差が大きくなる)

	static float EaseInSineP(float t, float b, float c, float d);
	static float EaseInSine(float x);

	static float EaseInCubicP(float t, float b, float c, float d);
	static float EaseInCubic(float x);

	static float EaseInQuintP(float t, float b, float c, float d);
	static float EaseInQuint(float x);

	static float EaseInCircP(float t, float b, float c, float d);
	static float EaseInCirc(float x);
	
	static float EaseInBackP(float t, float b, float c, float d);
	static float EaseInBack(float x);

public: // 中間地点が一番速い挙動(下にいくほど挙動の差が大きくなる)

	static float EaseOutSineP(float t, float b, float c, float d);
	static float EaseOutSine(float x);

	static float EaseOutCubicP(float t, float b, float c, float d);
	static float EaseOutCubic(float x);

	static float EaseOutQuintP(float time, float startPos, float endPos, float totalTime);
	static float EaseOutQuint(float x);

	static float EaseOutCircP(float t, float b, float c, float d);
	static float EaseOutCirc(float x);

	static float EaseOutBackP(float t, float b, float c, float d);
	static float EaseOutBack(float x);

public: // 波の挙動(下にいくほど挙動の差が大きくなる)

	static float EaseInOutSineP(float t, float b, float c, float d);
	static float EaseInOutSine(float x);

	static float EaseInOutCubicP(float t, float b, float c, float d);
	static float EaseInOutCubic(float x);

	static float EaseInOutQuintP(float t, float b, float c, float d);
	static float EaseInOutQuint(float x);

	static float EaseInOutCircP(float t, float b, float c, float d);
	static float EaseInOutCirc(float x);

	static float EaseInOutBackP(float t, float b, float c, float d);
	static float EaseInOutBack(float x);
};