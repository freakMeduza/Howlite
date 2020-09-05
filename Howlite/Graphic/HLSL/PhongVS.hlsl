#include "ConstantBuffer.hlsli"

struct ShaderInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Uv : UV;
};

struct ShaderOutput
{
    float3 Fragment : FRAGMENT;
    float3 Normal : NORMAL;
    float2 Uv : UV;
    float4 Position : SV_POSITION;
};

ShaderOutput main(ShaderInput Input)
{
    float4 position = float4(Input.Position, 1.0f);
    
    const float3 fragment = (float3)mul(position, ModelMatrix);
    const float3 normal = mul(Input.Normal, (float3x3) ModelMatrix);
    
    position = mul(position, ModelMatrix);
    position = mul(position, ViewMatrix);
    position = mul(position, ProjectionMatrix);
    
    ShaderOutput Output;

    Output.Fragment = fragment;
    Output.Normal = normal;
    Output.Uv = Input.Uv;
    Output.Position = position;
    
    return Output;
}
