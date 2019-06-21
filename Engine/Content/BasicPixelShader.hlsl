//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
////----------------------------------------------------------------------

Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);


struct sPSInput
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

float4 main(sPSInput input) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DiffuseSampler, input.tex);
	return diffuse;
}
