#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);

    return exp(-(d * d) / (2 * sigma * sigma));
}
            
float4 GaussianBlur(float2 uv, float sigma, float stepWidth)
{
    float totalWeight = 0;
    float _FocusDepth = 0.05;
    float _FocusWidth = 0.05;
    float4 col = float4(0, 0, 0, 0);
            
    for (float py = - sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = - sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float2 pickUV = uv + float2(px, py);
            float pickDepth = tex.Sample(smp, pickUV).r;
            float pickFocus = smoothstep(0, _FocusWidth, abs(pickDepth - _FocusDepth));

            float weight = Gaussian(uv, pickUV, sigma) * pickFocus;
            col += tex.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }
    col.rgb /= totalWeight;
    col.a = 1;
            
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	float _FocusDepth = 0.043;
    float _NFocusWidth = 0.01;
    float _FFocusWidth = 0.02;
    float4 depth = tex.Sample(smp, input.uv);
    float inFocus = 1 - smoothstep(0, _NFocusWidth, abs(depth - _FocusDepth));
    float outFocus = smoothstep(_NFocusWidth, _FFocusWidth, abs(depth - _FocusDepth));
    float middleFocus = 1 - inFocus - outFocus;
    float4 inFocusColor = tex.Sample(smp, input.uv);
    float4 middleFocusColor = GaussianBlur(input.uv, 0.03, 0.003);
    float4 outFocusColor = GaussianBlur(input.uv, 0.03, 0.004);
    //float4 inFocusColor = float4(1, 0, 0, 1);
    //float4 middleFocusColor = float4(0, 1, 0, 1);
    //float4 outFocusColor = float4(0, 0, 1, 1);
    
    return (inFocus * inFocusColor) + (middleFocus * middleFocusColor) + (outFocus * outFocusColor);
}