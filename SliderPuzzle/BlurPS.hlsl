Texture2D image;

SamplerState imageSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer PS_CONSTANT_BUFFER
{
	float2 direction;
	float2 padding;
};


float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{

	float Pixels[13] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
};

float BlurWeights[13] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

	
	// Pixel width/height
	float pixelWidth = 1.f / 1280.f;
	float pixelHeight = 1.f / 720.f;

	float4 color = { 0, 0, 0, 1 };

	float2 blur;
	if (direction.x > 0.f)
	{
		blur.y = uv.y;
	}
	else
	{
		blur.x = uv.x;
	}

	for (int i = 0; i < 13; i++)
	{
		if (direction.x > 0.f)
		{
			blur.x = uv.x + Pixels[i] * pixelWidth;
		}
		else
		{
			blur.y = uv.y + Pixels[i] * pixelHeight;
		}
		color = color + image.Sample(imageSampler, blur.xy) * BlurWeights[i];
	}
	
	color.a = 1;
	
	return color;
}