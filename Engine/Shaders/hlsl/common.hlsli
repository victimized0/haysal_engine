//************************* CONSTANTS

#define REG_CB_PER_DRAW		b0
#define REG_CB_PER_MATERIAL	b1
#define REG_CB_PER_PASS		b2
#define REG_CB_PER_FRAME	b3

//************************* CONSTANT BUFFERS

cbuffer CB_PerDraw : register(REG_CB_PER_DRAW)
{
	float4x4 WorldMatrix;
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

//************************* VERTEX SHADER INPUT

struct VS_IN_POS
{
	float3 Pos : POSITION;
};

struct VS_IN_POS_TEX
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct VS_IN_POS_COL_TEX
{
	float3 Pos : POSITION;
	float4 Col : COLOR;
	float2 Tex : TEXCOORD;
};

struct VS_IN_POS_NML_TEX
{
	float3 Pos : POSITION;
	float3 Nml : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_IN_POS_NML_COL_TEX
{
	float3 Pos : POSITION;
	float3 Nml : NORMAL;
	float3 Col : COLOR;
	float2 Tex : TEXCOORD;
};

//************************* PIXEL SHADER INPUT

struct VS_OUT
{
	float4 HPos		: SV_POSITION;
	float4 Color	: COLOR0;
};
