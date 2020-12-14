#include "common.hlsli"

VS_OUT DefaultVS( in VS_IN_POS input )
{
	VS_OUT output	= (VS_OUT)0;
	output.HPos		= mul( mul(ViewProjMatrix, WorldMatrix), float4(input.Pos.xyz, 1) );
	output.Color	= float4(1.f, 0.f, 0.f, 1.f);
	return output;
}

float4 DefaultPS(VS_OUT input) : SV_Target0
{
	return input.Color;
}
