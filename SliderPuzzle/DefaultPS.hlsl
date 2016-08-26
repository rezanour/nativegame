Texture2D image;
sampler imageSampler;

cbuffer PS_CONSTANT_BUFFER
{
    float2 tilePos;
    float tileSize;
    int enabled;
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_TARGET
{
    if (enabled && (pos.x < tilePos.x + 1 || pos.x > tilePos.x + tileSize - 1 ||
        pos.y < tilePos.y + 1 || pos.y > tilePos.y + tileSize - 1))
    {
        return float4(1, 1, 1, 1);
    }
    return image.Sample(imageSampler, uv);
}