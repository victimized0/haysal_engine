#include "stdafx.h"
#include "InputLayout.h"

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_Empty[] =
{
	{}
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_Pos[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_Pos, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VertexFormat_PosTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosColTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosColTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_COL, 0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, offsetof(VertexFormat_PosColTex, Col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VertexFormat_PosColTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosNmlTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosNmlTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_NML, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosNmlTex, Nml), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VertexFormat_PosNmlTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosNmlColTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosNmlColTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_NML, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VertexFormat_PosNmlColTex, Nml), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_COL, 0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, offsetof(VertexFormat_PosNmlColTex, Col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VertexFormat_PosNmlColTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const InputLayout::InputLayoutDesc InputLayout::InputLayoutDescs[] =
{
	{ COUNTOF(ElementDesc_Empty),			ElementDesc_Empty },
	{ COUNTOF(ElementDesc_Pos),				ElementDesc_Pos },
	{ COUNTOF(ElementDesc_PosTex),			ElementDesc_PosTex },
	{ COUNTOF(ElementDesc_PosColTex),		ElementDesc_PosColTex },
	{ COUNTOF(ElementDesc_PosNmlTex),		ElementDesc_PosNmlTex },
	{ COUNTOF(ElementDesc_PosNmlColTex),	ElementDesc_PosNmlColTex }
}