#include"AtmosphereScattering.hlsli"

//cbuffer ModelViewProjectionConstantBuffer : register(b0)
//{
//	matrix model;
//	matrix view;
//	matrix projection;
//
//	float time;
//};

half3 MixFog(half3 fragColor, float3 wsPos)
{
	half4 c1, c2, c3;

	ScatterSky(wsPos, c1, c2, c3);

	return c1.xyz;
};