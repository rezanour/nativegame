Texture2D image;

SamplerState imageSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer PS_CONSTANT_BUFFER
{
	float4 border;
	float borderWidth;
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD)
{
	float4 current = image.Sample(imageSampler, uv);
	float4 oneLeft = image.Sample(imageSampler, float2(uv.x - 1, uv.y));

	return float4((current.r + oneLeft.r) / 2, (current.g + oneLeft.g) / 2, (current.b + oneLeft.b) / 2, (current.a + oneLeft.a) / 2);
}