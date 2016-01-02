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
	float4 normal:   NORMAL;
	float2 texcoord: TEXCOORD;
};

struct PSInput
{
	float4 position:      SV_POSITION;
	float4 normal:        NORMAL;
	float3 viewDirection: TEXCOORD0;
	float2 texcoord:      TEXCOORD1;
};

PSInput main(VSInput input)
{
	PSInput output;
	output.position = mul(mvpMatrix, input.position);
	output.texcoord = input.texcoord;
	output.normal   = mul(rotationMatrix, input.normal);
	output.normal   = normalize(output.normal);

	float4 vertexWorldPosition = mul(worldMatrix, input.position);
	output.viewDirection       = eyePosition.xyz - vertexWorldPosition.xyz;
	output.viewDirection       = normalize(output.viewDirection);
	return output;
}