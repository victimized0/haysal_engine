#include "StdAfx.h"
#include "InputLayout.h"

InputLayout::InputLayout(std::vector<InputElementDesc>&& decs)
	: m_ElementsDesc(std::move(decs))
	, m_StartSlot(0)
{
	// Calculate strides
	m_Strides.reserve(m_ElementsDesc.size());
	for (const auto& dec : m_ElementsDesc)
	{
		const uint16 slot = dec.InputSlot;
		m_Strides[slot] = std::max(m_Strides[slot], uint16(dec.AlignedByteOffset + DeviceFormats::GetStride(dec.Format)));
	}

	// Calculate offsets
	m_Offsets[Offset_Position] = m_Offsets[Offset_Color] = m_Offsets[Offset_TexCoord] = m_Offsets[Offset_Normal] = -1;
	for (int n = 0; n < m_ElementsDesc.size(); ++n)
	{
		if (!m_ElementsDesc[n].SemanticName)
			continue;

		if ((m_Offsets[Offset_Position] == -1) && (_stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_POS) == 0))
			m_Offsets[Offset_Position] = m_ElementsDesc[n].AlignedByteOffset;
		if ((m_Offsets[Offset_Color] == -1) && (_stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_COL) == 0))
			m_Offsets[Offset_Color] = m_ElementsDesc[n].AlignedByteOffset;
		if ((m_Offsets[Offset_TexCoord] == -1) && (_stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_TEX) == 0))
			m_Offsets[Offset_TexCoord] = m_ElementsDesc[n].AlignedByteOffset;
		if ((m_Offsets[Offset_Normal] == -1) && (_stricmp(m_ElementsDesc[n].SemanticName, SEMANTIC_NML) == 0))
			m_Offsets[Offset_Normal] = m_ElementsDesc[n].AlignedByteOffset;
	}
}

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_Empty[] =
{
	{}
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_Pos[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_Pos, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VF_PosTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosColTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosColTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_COL, 0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, offsetof(VF_PosColTex, Col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VF_PosColTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosNmlTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosNmlTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_NML, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosNmlTex, Nml), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VF_PosNmlTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayout::ElementDesc_PosNmlColTex[] =
{
	{ SEMANTIC_POS, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosNmlColTex, Pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_NML, 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(VF_PosNmlColTex, Nml), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_COL, 0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, offsetof(VF_PosNmlColTex, Col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ SEMANTIC_TEX, 0, DXGI_FORMAT_R32G32_FLOAT,	0, offsetof(VF_PosNmlColTex, Tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const InputLayout::InputLayoutDesc InputLayout::InputLayoutDescs[] =
{
	{ COUNTOF(ElementDesc_Empty),			ElementDesc_Empty },
	{ COUNTOF(ElementDesc_Pos),				ElementDesc_Pos },
	{ COUNTOF(ElementDesc_PosTex),			ElementDesc_PosTex },
	{ COUNTOF(ElementDesc_PosColTex),		ElementDesc_PosColTex },
	{ COUNTOF(ElementDesc_PosNmlTex),		ElementDesc_PosNmlTex },
	{ COUNTOF(ElementDesc_PosNmlColTex),	ElementDesc_PosNmlColTex }
};
