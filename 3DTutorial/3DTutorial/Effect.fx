struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
};


cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};



VS_OUTPUT VS(float4 position : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(position, gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);
	return output;
}



float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);    
}
