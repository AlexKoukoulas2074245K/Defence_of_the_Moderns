cbuffer VSCBuffer
{
	float4x4 rotationMatrix;
    float4x4 worldMatrix;
    float4x4 mvpMatrix;
    float4   eyePosition;
};

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
	output.position = mul(mvpMatrix, input.position);
	output.texcoord = input.texcoord;
	output.normal   = mul(rotationMatrix, input.normal);
	output.normal   = normalize(output.normal);

	return output;
}