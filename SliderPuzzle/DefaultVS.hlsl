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

VertexOut main( Vertex input )
{
    VertexOut output;
    output.pos = float4(input.pos, 0, 1);
    output.uv = input.uv;
	return output;
}