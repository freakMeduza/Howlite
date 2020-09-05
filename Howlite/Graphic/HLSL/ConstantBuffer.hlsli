cbuffer TransformBuffer : register(b0)
{
    matrix ModelMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};

cbuffer LightBuffer : register(b1)
{
    float3 LightPosition;
    float3 LightColor;
    float LightIntencity;
    float LightKc;
    float LightKl;
    float LightKq;
};

cbuffer MaterialBuffer : register(b2)
{
    float3 MaterialColor;
    float MaterialSpecularPower;
    float MaterialSpecularIntensity;
    bool DiffuseMapEnabled;
    bool NormalMapEnabled;
};

cbuffer CameraBuffer : register(b3)
{
    float3 CameraPosition;
}