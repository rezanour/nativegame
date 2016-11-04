struct Vertex
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER
{
    float2 pixelOffset;
    float2 texOffset;
    float2 screenSize;
    float2 stretch;
};


VertexOut main( Vertex input )
{
    VertexOut output;
    float2 offset;
    offset.x = pixelOffset.x / screenSize.x * 2;
    offset.y = -(pixelOffset.y / screenSize.y * 2);
    float2 newPos = input.pos + offset;
    output.pos = float4(newPos, 0, 1);
    output.uv = input.uv + texOffset;
	return output;
}