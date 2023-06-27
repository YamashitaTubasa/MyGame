#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
  
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    float luminance = 1.0f; // ‹P“x
    float totalWeight = 0;
    float _Sigma = 0.005;
    float _StepWidth = 0.001;

    float4 col = tex.Sample(smp, input.uv) * color;

    if (isBlur == true)
    {
        for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
        {
            for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
            {
                float2 pickUV = input.uv + float2(px, py);
                float weight = Gaussian(input.uv, pickUV, _Sigma);
                col += tex.Sample(smp, pickUV) * weight;
                totalWeight += weight;
            }
        }
        col.rgb /= totalWeight;
        col.a = 1;
    }
    
    return float4(col.rgb * luminance, alpha);
}

