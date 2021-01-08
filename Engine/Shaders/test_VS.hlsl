cbuffer transform : register(b0)
{
    matrix world_matrix;
    matrix view_matrix;
    matrix projection_matrix;
};

struct vs_out {
    float3 color : COLOR;
    float4 position : SV_POSITION;
};

vs_out main(float3 position : POSITION, float3 color : COLOR)
{
    vs_out output;
    output.position = float4(position, 1.0f);
    output.position = mul(output.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, projection_matrix);
    output.color = color;
    return output;
}