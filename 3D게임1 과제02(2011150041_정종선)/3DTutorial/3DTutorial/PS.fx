struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}
