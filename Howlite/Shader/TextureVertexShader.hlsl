cbuffer TransformBuffer
{
    matrix ModelMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};

struct Result
{
    float2 uv       : UV;
    float4 position : SV_Position;
};

Result main(float3 position : Position, float2 uv : UV)
{
    Result result;
    float4 p = float4(position, 1.0f);
    p = mul(p, ModelMatrix);
    p = mul(p, ViewMatrix);
    p = mul(p, ProjectionMatrix);
    result.position = p;
    result.uv = uv;
    return result;
}