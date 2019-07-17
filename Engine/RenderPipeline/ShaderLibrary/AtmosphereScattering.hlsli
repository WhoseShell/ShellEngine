
#define M_PI 3.141592657f

cbuffer AtmosCB : register(b0)
{
	matrix null0;
	matrix null1;
	matrix null2;

	float4 null3;

	float3		u_CameraPosition;//相机世界坐标位置
	float3		u_SunDirection;
	float3		u_RayleighColorM20;
	float3		u_RayleighColorM10;
	float3		u_RayleighColorMP0;
	float3		u_RayleighColorP10;
	float3		u_RayleighColorP20;

	float3		u_MieColorM20;
	float3		u_MieColorMP0;
	float3		u_MieColorP20;

	float		u_HeightNormalDistanceRcp;
	float		u_HeightNearScatterPush;
	float		u_HeightRayleighDensity;
	float		u_HeightMieDensity;

	float		u_HeightSeaLevel;
	float3		u_HeightPlaneShift;

	float		u_HeightDistanceRcp;
	float		u_WorldScaleExponent;
	float		u_WorldNearScatterPush;
	float		u_WorldRayleighDensity;

	float		u_WorldMieDensity;
	float		u_WorldNormalDistanceRcp;
	float2		u_RayleighInScatterPct;

	float3		u_HeightRayleighColor;

	float		raylieHeightDensity;
	float		u_MiePhaseAnisotropy;
	float		u_MieColorIntensity;

};




struct ScatterInput {
	float2 pos;
	half4 scatterCoords1;
	half3 scatterCoords2;
};

// Convert rgb to luminance
// with rgb in linear space with sRGB primaries and D65 white point
half Luminance(half3 linearRgb)
{
	return dot(linearRgb, half3(0.2126729, 0.7151522, 0.0721750));
}


float henyeyGreenstein(float g, float cosTheta) {
	float gSqr = g * g;
	float a1 = (1.f - gSqr);
	float a2 = (2.f + gSqr);
	float b1 = 1.f + cosTheta * cosTheta;
	float b2 = abs(pow(1.f + gSqr - 2.f * g * cosTheta, 1.5f));
	return (a1 / a2) * (b1 / b2);
};

float rayleighPhase(float cosTheta) {
	const float f = 3.f / (16.f * M_PI);
	return f + f * cosTheta * cosTheta;
};

float miePhase(float cosTheta, float anisotropy) {
	const float f = 3.f / (8.f * M_PI);
	return f * henyeyGreenstein(anisotropy, cosTheta);
};

float heightDensity(float h, float H) {
	return exp(-h / H);
};

float3 WorldScale(float3 p) {
	p.xz = sign(p.xz) * pow(abs(p.xz), 1.0f);
	return p;
};

void _VolundTransferScatter(float3 _worldPos, out half4 coords1, out half4 coords2, out half4 coords3) {

	/*float3		u_RayleighColorM20 = float3(0.758f, 0.612f, 0.377f);
	float3		u_RayleighColorM10 = float3(0.758f, 0.612f, 0.377f);
	float3		u_RayleighColorMP0 = float3(0.758f, 0.612f, 0.377f);
	float3		u_RayleighColorP10 = float3(1.0f, 0.63f, 0.17f);
	float3		u_RayleighColorP20 = float3(0.758f, 0.612f, 0.377f);*/

	float3		u_MieColorM20 = float3(1.0f, 0.612f, 0.13f);
	float3		u_MieColorMP0 = float3(1.0f, 0.64f, 0.13f);
	float3		u_MieColorP20 = float3(1.0f, 0.612f, 0.12f);

	float		u_HeightNormalDistanceRcp = 1.0f / 1000.0f;;
	float		u_HeightNearScatterPush = 0;
	float		u_HeightRayleighDensity = -0 / 100000.0f;
	float		u_HeightMieDensity = 0;
	float		u_HeightSeaLevel = 0;
	float3		u_HeightPlaneShift = 0;
	float		u_HeightDistanceRcp = 1.0f / 50.0f;

	float		u_WorldScaleExponent = 1.0f;
	float		u_WorldNearScatterPush = 0.0f;
	float		u_WorldRayleighDensity = -2000.0f / 100000.0f;
	float		u_WorldMieDensity = -200.9f / 100000.0f;
	float		u_WorldNormalDistanceRcp = 1.0f / 1000.0f;
	float		raylieHeightDensity = 1.23f;

	float2		u_RayleighInScatterPct = float2(0.4f, 0.6f);

	float		u_MiePhaseAnisotropy = 0.8f;
	float		u_MieColorIntensity = 2.0f;
	float4		u_HeightRayleighColor = float4(0.97f, 0.97f, 0.97f, 1.0f);



	const float3 worldPos = WorldScale(_worldPos);
	const float3 worldCamPos = WorldScale(u_CameraPosition.xyz);

	const float c_MieScaleHeight = 1200.f;
	const float worldRayleighDensity = 1.f;
	const float worldMieDensity = heightDensity(worldPos.y, c_MieScaleHeight);

	const float3 worldVec = worldPos.xyz - worldCamPos.xyz;
	const float worldVecLen = length(worldVec);
	const float3 worldDir = worldVec / worldVecLen;

	const float3 worldDirUnscaled = normalize(_worldPos - u_CameraPosition.xyz);

	const float viewSunCos = dot(worldDirUnscaled, u_SunDirection);
	const float rayleighPh = min(1.f, rayleighPhase(viewSunCos) * 12.f);
	const float miePh = miePhase(viewSunCos, u_MiePhaseAnisotropy);

	const float angle20 = 0.324f / 1.5f;
	const float angle10 = 0.174f / 1.5f;
	const float angleY = worldDir.y * saturate(worldVecLen / 250.0);

	float3 rayleighColor;
	if (angleY >= angle10) rayleighColor = lerp(u_RayleighColorP10, u_RayleighColorP20, saturate((angleY - angle10) / (angle20 - angle10)));
	else if (angleY >= 0.f) rayleighColor = lerp(u_RayleighColorMP0, u_RayleighColorP10, angleY / angle10);
	else if (angleY >= -angle10) rayleighColor = lerp(u_RayleighColorM10, u_RayleighColorMP0, (angleY + angle10) / angle10);
	else rayleighColor = lerp(u_RayleighColorM20, u_RayleighColorM10, saturate((angleY + angle20) / (angle20 - angle10)));

	float3 mieColor;
	if (angleY >= 0.f) mieColor = lerp(u_MieColorMP0, u_MieColorP20, saturate(angleY / angle20));
	else mieColor = lerp(u_MieColorM20, u_MieColorMP0, saturate((angleY + angle20) / angle20));

	const float pushedDistance = max(0.f, worldVecLen + u_WorldNearScatterPush);//一段距离内没有雾效
	const float pushedDensity = /*heightDensity **/ pushedDistance * exp(-worldPos.y / (raylieHeightDensity * 1000.0f));//雾气衰减路径的积分，此处用的简单模型
	const float rayleighScatter = (1.f - exp(u_WorldRayleighDensity * pushedDensity)) * rayleighPh;
#ifdef IS_RENDERING_SKY
	const float mieScatter = (1.f - exp(u_WorldMieDensity * pushedDistance));
#else
	const float mieScatter = (1.f - exp(u_WorldMieDensity * pushedDistance)) * miePh;
#endif

	const float heightShift = dot(worldVec, u_HeightPlaneShift);
	const float heightScaledOffset = (worldPos.y - heightShift - /*u_HeightSeaLevel*/0) * u_HeightDistanceRcp;
	const float heightDensity = exp(-heightScaledOffset);
	const float pushedHeightDistance = max(0.f, worldVecLen + u_HeightNearScatterPush);
	const float heightScatter = (1.f - exp(u_HeightRayleighDensity * pushedHeightDistance)) * heightDensity;
#ifdef IS_RENDERING_SKY
	const float heightMieScatter = (1.f - exp(u_HeightMieDensity * pushedHeightDistance)) * heightDensity;
#else
	const float heightMieScatter = (1.f - exp(u_HeightMieDensity * pushedHeightDistance)) * heightDensity * miePh;
#endif

	rayleighColor = lerp(Luminance(rayleighColor).rrr, rayleighColor, saturate(pushedDistance * u_WorldNormalDistanceRcp));
	float3 heightRayleighColor = lerp(Luminance(u_HeightRayleighColor.xyz).rrr, u_HeightRayleighColor.xyz, saturate(pushedHeightDistance * u_HeightNormalDistanceRcp));

	coords1.rgb = rayleighScatter * rayleighColor;
	coords1.a = rayleighScatter;

	coords3.rgb = saturate(heightScatter) * heightRayleighColor;
	coords3.a = heightScatter;

	coords2.rgb = (mieScatter+0.05f) * u_MieColorIntensity * mieColor;
	coords2.a = mieScatter;
}

void VolundTransferScatter(float3 worldPos, out half4 coords1) {
	half4 c1, c2, c3;
	_VolundTransferScatter(worldPos, c1, c2, c3);


	float		u_RayleighExtinctionFactor = 1.0f;
	float		u_HeightExtinctionFactor = 0;
	float		u_MieExtinctionFactor = 1.0f;

#ifdef IS_RENDERING_SKY
	coords1.rgb = c3.rgb;
	coords1.a = max(0.f, 1.f - c3.a * u_HeightExtinctionFactor);
#else
	coords1.rgb = c1.rgb;
	coords1.rgb += c3.rgb;
	coords1.a = max(0.f, 1.f - c1.a * u_RayleighExtinctionFactor - c3.a * u_HeightExtinctionFactor);
#endif

	coords1.rgb += c2.rgb;
	coords1.a *= max(0.f, 1.f - c2.a * u_MieExtinctionFactor);
}

half3 VolundApplyScatter(half4 coords1, half2 pos, half3 color) 
{
	return color * coords1.a + coords1.rgb;
}

#define VOLUND_TRANSFER_SCATTER(pos, o) o.scatterCoords1 = pos.xyzz;
#define VOLUND_APPLY_SCATTER(i, color) VolundTransferScatter(i.scatterCoords1.xyz, i.scatterCoords1); color.xyz = VolundApplyScatter(i.scatterCoords1, i.pos.xy, color.xyz);