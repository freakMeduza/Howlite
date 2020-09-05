#include "ConstantBuffer.hlsli"

struct ShaderInput
{
    float3 Position : POSITION;
};

struct ShaderOutput
{
    float4 Position : SV_POSITION;
};

ShaderOutput main(ShaderInput Input) 
{
    float4 position = float4(Input.Position, 1.0f);
    
    position = mul(position, ModelMatrix);
    position = mul(position, ViewMatrix);
    position = mul(position, ProjectionMatrix);
    
    ShaderOutput Output;
    
    Output.Position = position;
    
    return Output;
}