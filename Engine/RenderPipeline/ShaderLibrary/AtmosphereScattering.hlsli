
#define M_PI 3.141592657f

cbuffer AtmosCB : register(b0)
{
	matrix null1;
	matrix null2;
	matrix null3;
	float null4;

	float3		u_CameraPosition;//相机世界坐标位置
	float3		u_SunDirection;

	float		u_WorldScaleExponent; //世界坐标缩放
	float		u_WorldNearScatterPush;
	float		u_WorldRayleighDensity;
	float		u_WorldMieDensity;
	float		raylieHeightDensity;

	float3		u_RayleighColorM20;
	float3		u_RayleighColorM10;
	float3		u_RayleighColorO00;
	float3		u_RayleighColorP10;
	float3		u_RayleighColorP20;
	float3		u_RayleighColorP45;

	float3		u_MieColorM20;
	float3		u_MieColorO00;
	float3		u_MieColorP20;
	float3		u_MieColorP45;

	float u_MieColorIntensity;
	float		u_MiePhaseAnisotropy;
	//float		u_MieCoeffScale;
	//float3		u_MieSunTintIntensity;

	//float		u_RayleighCoeffScale;
	//float3		u_RayleighSunTintIntensity;
	//float2		u_RayleighInScatterPct;

};

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
	p.xz = sign(p.xz) * pow(abs(p.xz), u_WorldScaleExponent);
	return p;
};

void ScatterSky(float3 _worldPos, out half4 coords1, out half4 coords2, out half4 coords3)
{
	half4 c1, c2, c3;
	coords1 = c1;
	coords2 = c2;
	coords3 = c3;



	const float3 worldPos = WorldScale(_worldPos);
	const float3 worldCamPos = WorldScale(u_CameraPosition.xyz);

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
	else if (angleY >= 0.f) rayleighColor = lerp(u_RayleighColorO00, u_RayleighColorP10, angleY / angle10);
	else if (angleY >= -angle10) rayleighColor = lerp(u_RayleighColorM10, u_RayleighColorO00, (angleY + angle10) / angle10);
	else rayleighColor = lerp(u_RayleighColorM20, u_RayleighColorM10, saturate((angleY + angle20) / (angle20 - angle10)));

	float3 mieColor;
	if (angleY >= 0.f) mieColor = lerp(u_MieColorO00, u_MieColorP20, saturate(angleY / angle20));
	else mieColor = lerp(u_MieColorM20, u_MieColorO00, saturate((angleY + angle20) / angle20));

	const float pushedDistance = max(0.f, worldVecLen + u_WorldNearScatterPush);//一段距离内没有雾效
	const float pushedDensity = /*heightDensity **/ pushedDistance * exp(-worldPos.y / (raylieHeightDensity * 1000.0f));//雾气衰减路径的积分，此处用的简单模型



	/***********************************************************************************************/
	float atmosThickness = raylieHeightDensity * 1000.0f;
	float Tcp = pushedDistance + (atmosThickness / worldPos.y) * exp(-u_WorldRayleighDensity * ((0.5 * worldPos.y + worldCamPos.y) / atmosThickness)) - atmosThickness / worldCamPos.y;

	const float rayleighScatter = Tcp * rayleighPh;


	/***********************************************************************************************/

#ifdef IS_RENDERING_SKY
	const float mieScatter = (1.f - exp(u_WorldMieDensity * pushedDistance));
#else
	const float mieScatter = (1.f - exp(u_WorldMieDensity * pushedDistance)) * miePh;
#endif

	coords1.rgb = rayleighScatter * rayleighColor;
	coords1.a = rayleighScatter;

	coords2.rgb = mieScatter * mieColor * u_MieColorIntensity;
	coords2.a = mieScatter;
};

void Scatter(float3 _worldPos, out half4 coords1, out half4 coords2, out half4 coords3)
{
	half4 c1, c2, c3;
	coords1 = c1;
	coords2 = c2;
	coords3 = c3;
};