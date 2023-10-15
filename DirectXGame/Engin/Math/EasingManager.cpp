/**
 * @file EasingManager.cpp
 * @brief イージングの管理クラス
 * @author Yamashita Tubasa
 */

#include "EasingManager.h"

#include <math.h>

#define PI 3.14159265359

float EasingManager::EaseInSineP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInSine(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInSine(float x)
{
	return static_cast<float>(1 - cos((x * PI) / 2));
}

float EasingManager::EaseInCubicP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInCubic(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInCubic(float x)
{
	return static_cast<float>(x * x * x);
}

float EasingManager::EaseInQuintP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInQuint(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInQuint(float x)
{
	return static_cast<float>(x * x * x * x * x);
}

float EasingManager::EaseInCircP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInCirc(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInCirc(float x)
{
	return static_cast<float>(1 - sqrt(1 - pow(x, 2)));
}

float EasingManager::EaseInBackP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInBack(x);
	float result = v * c + b;

	return result;
}

float EasingManager::EaseInBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 + 1;

	return static_cast<float>(c2 * x * x * x - c1 * x * x);
}

float EasingManager::EaseOutSineP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutSine(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseOutSine(float x)
{
	return static_cast<float>(sin((x * PI) / 2));
}

float EasingManager::EaseOutCubicP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutCubic(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseOutCubic(float x)
{
	return static_cast<float>(1 - pow(1 - x, 3));
}

float EasingManager::EaseOutQuintP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutQuint(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseOutQuint(float x)
{
	return static_cast<float>(1 - pow(1 - x, 5));
}

float EasingManager::EaseOutCircP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutCirc(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseOutCirc(float x)
{
	return static_cast<float>(sqrt(1 - pow(x - 1, 2)));
}

float EasingManager::EaseOutBackP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseOutBack(x);
	float result = v * c + b;

	return result;
}

float EasingManager::EaseOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 + 1;

	return static_cast<float>(1 + c2 * pow(x - 1, 3) + c1 * pow(x - 1, 2));
}

float EasingManager::EaseInOutSineP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInOutSine(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInOutSine(float x)
{
	return static_cast<float>(-(cos(PI * x) - 1) / 2);
}

float EasingManager::EaseInOutCubicP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInOutCubic(x);
	float result = c * v + b;

	return result;
}

float EasingManager::EaseInOutCubic(float x)
{
	return static_cast<float>(x < 0.5 
		? 4 * x * x * x 
		: 1 - pow(-2 * x + 2, 3) / 2);
}

float EasingManager::EaseInOutQuintP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInOutQuint(x);
	float result = v * c + b;

	return result;
}

float EasingManager::EaseInOutQuint(float x)
{
	return static_cast<float>(x < 0.5 
		? 16 * x * x * x * x * x 
		: 1 - pow(-2 * x + 2, 5) / 2);
}

float EasingManager::EaseInOutCircP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInOutCirc(x);
	float result = v * c + b;

	return result;
}

float EasingManager::EaseInOutCirc(float x)
{
	return static_cast<float>(x < 0.5 
		? (1 - sqrt(1 - pow(2 * x, 2)))/ 2 
		: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2);
}

float EasingManager::EaseInOutBackP(float t, float b, float c, float d)
{
	float x = t / d;
	float v = EaseInOutBack(x);
	float result = v * c + b;

	return result;
}

float EasingManager::EaseInOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return static_cast<float>(x < 0.5 
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2);
}
