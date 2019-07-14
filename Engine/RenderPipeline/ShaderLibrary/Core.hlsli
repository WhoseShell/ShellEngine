#include"AtmosphereScattering.hlsli"

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float time;
};

half3 MixFog(half3 fragColor, float3 wsPos)
{
	return fragColor;
};