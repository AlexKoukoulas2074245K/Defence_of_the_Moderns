struct VSInput
{
	float4 position: POSITION;
	float2 texcoord: TEXCOORD;
	float4 normal:   NORMAL;
};

struct PSInput
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD;
	float4 normal:   NORMAL;
};

PSInput main(VSInput input)
{
	PSInput output;
	output.position = input.position;
	output.texcoord = input.texcoord;
	output.normal   = input.normal;

	return output;
}