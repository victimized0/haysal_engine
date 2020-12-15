#ifndef INTERFACE_SHADER_H
#define INTERFACE_SHADER_H
#pragma once

struct IShader;
struct IShaderResources;
struct ITexture;
struct ShaderParam;
struct ShaderItem;
struct ShaderBlob;
struct ShaderInfo;
struct ShaderTextureSlot;
struct ShaderTextureSlots;
//class RenderElement;

enum ShaderFlags : uint16
{
	Default			= BIT16(0),
	NoDraw			= BIT16(1),
	NoShadows		= BIT16(2),
	UseLights		= BIT16(3),
	PostProcess		= BIT16(4),
	SystemShader	= BIT16(5),
};

enum class ShaderType : uint8
{
	General		= 0,
	Light		= 1,
	Shadow		= 2,
	Terrain		= 3,
	NoDraw		= 4,
	Sky			= 5,
};

enum class TechniqueId
{
	General = 0,
	Depth = 1,
	Shadow = 2,
	Glow = 3,
	EarlyZ = 4,

	Total
};

enum class ShaderResourceTextureSlot
{
	Diffuse		= 0,
	Normal		= 1,
	Specular	= 2,
	Environment	= 3,

	Total,
	Unknown		= Total
};

enum class CullType : uint8
{
	Back,
	Front,
	None
};

struct IShaderResources
{
	virtual								~IShaderResources() {}

	virtual void						AddRef() const = 0;
	virtual void						UpdateConstants(IShader* pSH) = 0;

	virtual const Color&				GetColor(ShaderResourceTextureSlot slot) const = 0;
	virtual void						SetColor(ShaderResourceTextureSlot slot, const Color& color) = 0;

	virtual float						GetStrength(ShaderResourceTextureSlot slot) const = 0;
	virtual void						SetStrength(ShaderResourceTextureSlot slot, float value) = 0;

	virtual const float&				GetAlphaRef() const = 0;
	virtual void						SetAlphaRef(float v) = 0;

	virtual int							GetFlags() = 0;
	virtual void						SetMaterialName(const char* name) = 0;
	virtual class Texture*				GetTexture(int slot) const = 0;
	virtual std::vector<ShaderParam>&	GetParameters() = 0;

	virtual void						Release() const = 0;
	virtual void						ConvertToInputResource(struct InputShaderResources* pDst) = 0;
	virtual IShaderResources*			Clone() const = 0;
	virtual void						SetShaderParams(struct InputShaderResources* pDst, IShader* pShader) = 0;

	virtual void						SetInvalid() = 0;
	virtual bool						IsValid() = 0;

	inline bool							IsAlphaTested() const
	{
		return GetAlphaRef() > 0.01f;
	}
};

struct ShaderItem
{
	ShaderItem(IShader* pShader = nullptr, IShaderResources* pShaderResources = nullptr, int techId = -1)
		: m_pShader(pShader)
		, m_pShaderResources(pShaderResources)
		, m_techniqueId(techId)
		, m_preprocessFlags(1)
	{}

	uint32 PostLoad();
	bool   Update();
	//bool   RefreshResourceConstants();

	inline bool IsAlphaTested() const { return (m_pShaderResources && m_pShaderResources->IsAlphaTested()); }

	inline ShaderTechnique* GetTechnique() const { assert(false); /*Not implemented*/ return nullptr; }

private:
	IShader*				m_pShader;
	IShaderResources*		m_pShaderResources;
	int32                   m_techniqueId;
	uint32                  m_preprocessFlags;

};

struct IShader
{
	virtual									~IShader() {}

	virtual int								GetId() const = 0;
	virtual int								AddRef() = 0;
	virtual int								Release() = 0;
	virtual int								ForceRelease() = 0;

	virtual const char*						GetName() = 0;
	virtual const char*						GetName() const = 0;
	virtual uint32							GetFlags() const = 0;
	virtual void							SetFlags(uint32 flags) = 0;
	virtual void							ClearFlags(uint32 flags) = 0;

	virtual bool							Reload() = 0;

	//virtual std::vector<RenderElement*>*	GetRenderElements(int techniqueId) = 0;

	virtual ShaderInfo						GetInfo() const = 0;
	virtual void							SetInfo(ShaderInfo& info) = 0;

	virtual int								GetTexId() = 0;
	virtual ITexture*						GetBaseTexture(int* passNum, int* nTU) = 0;
	virtual uint32							GetUsedTextureTypes() = 0;
	virtual ShaderTextureSlots*				GetUsedTextureSlots(int techniqueId) = 0;

	virtual int								GetTechniqueId(int techniqueIndex) = 0;
	virtual VertexFormat					GetVertexFormat() = 0;
	virtual CullType						GetCullType() const = 0;
	virtual ShaderType						GetShaderType() const = 0;
	virtual ShaderBlob*						GetShaderBlob() = 0;

	static uint32							GetTextureSlotNum(ShaderResourceTextureSlot textureType) { return static_cast<uint32>(textureType); }
};

#endif //INTERFACE_SHADER_H
