cbuffer ColorBuffer
{
    float4 Color;
};

float4 main() : SV_Target
{
    return Color;
}