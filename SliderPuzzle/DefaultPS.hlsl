Texture2D image;
sampler imageSampler;

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
    return image.Sample(imageSampler, uv);
}