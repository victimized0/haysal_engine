#ifndef VERTEX_FORMATS_H
#define VERTEX_FORMATS_H
#pragma once

enum class VertexFormat : uint8
{
	None			= 0,
	Pos				= 1,
	PosTex			= 2,
	PosColTex		= 3,
	PosNmlTex		= 4,
	PosNmlColTex	= 5,

	Total
};

struct VertexFormat_Pos
{
	Vec3	Pos;
};

struct VertexFormat_PosTex
{
	Vec3	Pos;
	Vec2	Tex;
};

struct VertexFormat_PosColTex
{
	Vec3	Pos;
	Color	Col;
	Vec2	Tex;
};

struct VertexFormat_PosNmlTex
{
	Vec3	Pos;
	Vec3	Nml;
	Vec2	Tex;
};

struct VertexFormat_PosNmlColTex
{
	Vec3	Pos;
	Vec3	Nml;
	Color	Col;
	Vec2	Tex;
};

#endif //VERTEX_FORMATS_H
