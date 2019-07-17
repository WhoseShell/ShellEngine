#include"AtmosphereScattering.hlsli"

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float4 time;
};


half3 MixFog(half3 fragColor, float3 wsPos)
{
	ScatterInput si = (ScatterInput)0;
	VOLUND_TRANSFER_SCATTER(wsPos.xyz, si);
	VOLUND_APPLY_SCATTER(si, fragColor);
	return fragColor;
};