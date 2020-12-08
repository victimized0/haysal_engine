#define REG_CB_PER_DRAW		b0
#define REG_CB_PER_MATERIAL	b1
#define REG_CB_PER_PASS		b2
#define REG_CB_PER_FRAME	b3

cbuffer CB_PerDraw : register(REG_CB_PER_DRAW)
{
	float3x4 WorldMatrix;
};

cbuffer CB_PerMaterial : register(REG_CB_PER_MATERIAL)
{

};

cbuffer CB_PerPass : register(REG_CB_PER_PASS)
{

};

cbuffer CB_PerFrame : register(REG_CB_PER_FRAME)
{
	float4x4	ViewMatrix;
	float4x4	InvViewMatrix;
	float4x4	ProjMatrix;
	float4x4	InvProjMatrix;
	float4x4	ViewProjMatrix;
	float4x4	InvViewProjMatrix;
	float4		CamPos;
	float4		CamUpVec;
	float4		CamRightVec;
	float4		CamFrontVec;
	float4		SunDir;
	float4		SkyCol;
};

cbuffer CB_PerShadowPass : register(REG_CB_PER_PASS)
{
	float4 LightPos;
	float4 ViewPos;
	float4 DSBias;
};
