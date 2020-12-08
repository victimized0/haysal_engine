#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <RenderModule\IShader.h>

class Shader;
struct ShaderPass;
struct ShaderTechnique;

struct ShaderParam
{
	enum Type : uint8
	{
		Unknown,
		Byte,
		Bool,
		Short,
		Int,
		//Long,
		Half,
		Single,
		//Double,
		Color,
		Vector
	};

	union Value
	{
		byte	Byte;
		bool	Bool;
		int16	Short;
		int32	Int;
		//int64	Long;
		float	Single;
		//double	Double;
		float	Color[4];
		float	Vector[3];
	};

	std::string	m_Name;
	Type		m_Type;
	Value		m_Value;
	uint8		m_Semantic;

	ShaderParam()
		: m_Type(Type::Unknown)
		, m_Semantic(0)
	{
		ZeroMemory(&m_Value, sizeof(m_Value));
	}
	~ShaderParam() = default;

	ShaderParam(const ShaderParam& src)
		: m_Name(src.m_Name)
		, m_Type(src.m_Type)
		, m_Value(src.m_Value)
		, m_Semantic(src.m_Semantic)
	{}
	ShaderParam& operator=(const ShaderParam& src)
	{
		m_Name = src.m_Name;
		m_Type = src.m_Type;
		m_Value = src.m_Value;
		m_Semantic = src.m_Semantic;

		return *this;
	}

	static bool SetParam(const char* name, std::vector<ShaderParam>* params, const Value& val)
	{
		size_t i = 0;
		for (; i < params->size(); ++i)
		{
			ShaderParam* sp = &(*params)[i];
			if (sp == nullptr || &sp->m_Value == &val) continue;

			if (sp->m_Name != name)
			{
				switch (sp->m_Type)
				{
				case Type::Half:
				case Type::Single:
					sp->m_Value.Single = val.Single;
					break;
				case Type::Short:
					sp->m_Value.Short = val.Short;
					break;
				case Type::Int:
					sp->m_Value.Int = val.Int;
					break;
				case Type::Vector:
					sp->m_Value.Vector[0] = val.Vector[0];
					sp->m_Value.Vector[1] = val.Vector[1];
					sp->m_Value.Vector[2] = val.Vector[2];
					break;
				case Type::Color:
					sp->m_Value.Color[0] = val.Color[0];
					sp->m_Value.Color[1] = val.Color[1];
					sp->m_Value.Color[2] = val.Color[2];
					sp->m_Value.Color[3] = val.Color[3];
					break;
				case Type::Bool:
					sp->m_Value.Bool = val.Bool;
					break;
				case Type::Byte:
					sp->m_Value.Byte = val.Byte;
					break;
				case Type::Unknown:
					assert(false);
					break;
				}
				break;
			}
		}
		if (i == params->size()) return false;
		return true;
	}
	static bool GetValue(const char* name, std::vector<ShaderParam>* params, float* v, int id);
	static bool GetValue(uint8 eSemantic, std::vector<ShaderParam>* params, float* v, int id);

	inline void CopyValue(const ShaderParam& src)
	{
		m_Value = src.m_Value;
	}
	inline void CopyType(const ShaderParam& src)
	{
		m_Type = src.m_Type;
	}
	inline size_t Size()
	{
		return sizeof(*this);
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

	inline ShaderTechnique* GetTechnique() const;

private:
	IShader*				m_pShader;
	IShaderResources*		m_pShaderResources;
	int32                   m_techniqueId;
	uint32                  m_preprocessFlags;

};

struct ShaderTextureSlot
{
	std::string	Name;
	std::string	Description;
	TextureType	Type;

	ShaderTextureSlot()
		: Type(TextureType::Unknown)
	{}
};

struct ShaderTextureSlots
{
	int					RefCount;
	ShaderTextureSlot*	UsedSlots[static_cast<int>(ShaderResourceTextureSlot::Total)];

	ShaderTextureSlots()
		: RefCount(1)
	{
		for (size_t i = 0; i < static_cast<int>(ShaderResourceTextureSlot::Total); ++i)
		{
			UsedSlots[i] = nullptr;
		}
	}

	~ShaderTextureSlots()
	{
		for (size_t i = 0; i < static_cast<int>(ShaderResourceTextureSlot::Total); ++i)
		{
			SAFE_DELETE(UsedSlots[i]);
		}
	}

	void Release()
	{
		RefCount--;
		if (RefCount == 0)
			delete this;
	}
};

struct ShaderTechnique
{
	//Shader*						m_pShader;
	std::string                 m_Name;
	std::vector<ShaderPass>     m_Passes;
	uint32                      m_Flags;
	uint32						m_PreprocessFlags;
	int8						m_TechniqueId[static_cast<int>(TechniqueId::Total)];
	std::vector<RenderElement*>	m_RenderElements;
	//std::vector<RenderTarget*>	m_RenderTargets;

	ShaderTechnique(/*Shader* pShader*/)
		: /*m_pShader(pShader)
		,*/ m_Flags(0)
		, m_PreprocessFlags(0)
	{
	}

	~ShaderTechnique()
	{
		for (size_t i = 0; i < m_Passes.size(); ++i)
		{
			ShaderPass* pPass = &m_Passes[i];
			pPass->Free();
		}

		for (size_t i = 0; i < m_RenderElements.size(); ++i)
		{
			RenderElement* pElement = m_RenderElements[i];
			pElement->Release(false);
		}

		m_RenderElements.clear();
		m_Passes.clear();
	}
};

struct ShaderPass : NoCopyNoMove
{
	uint8       m_AlphaRef;
	uint16      m_Flags;

	Shader*		m_pVertexShader;
	Shader*		m_pPixelShader;
	Shader*		m_pGeometryShader;
	Shader*		m_pComputeShader;

	ShaderPass();

	void Free()
	{
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pGeometryShader);
		SAFE_RELEASE(m_pComputeShader);
	}

	void AddRefsToShaders()
	{
		if (m_pVertexShader)
			m_pVertexShader->AddRef();
		if (m_pPixelShader)
			m_pPixelShader->AddRef();
		if (m_pGeometryShader)
			m_pGeometryShader->AddRef();
		if (m_pComputeShader)
			m_pComputeShader->AddRef();
	}
};

class Shader : public IShader, public BaseResource
{
public:
											Shader();
	virtual									~Shader();

	// BaseResource Interface
	virtual int								Release() override;
	virtual int								AddRef() override					{ return BaseResource::AddRef(); }
	virtual int								GetId() const override				{ return BaseResource::GetId(); }
	// ~BaseResource Interface

	// IShader Interface
	virtual const char*						GetName() override					{ return m_shaderName.c_str(); }
	virtual const char*						GetName() const override			{ return m_shaderName.c_str(); }
	virtual uint32							GetFlags() const override			{ return m_flags; }
	virtual void							SetFlags(uint32 flags) override		{ m_flags |= flags; }
	virtual void							ClearFlags(uint32 flags) override	{ m_flags &= ~flags; }
	virtual CullType						GetCullType() const override		{ return m_cullType; }
	virtual ShaderType						GetShaderType() const override		{ return m_shaderType; }
	virtual ShaderBlob*						GetShaderBlob() override			{ return &m_shaderBlob; }

	virtual int								ForceRelease() override;
	virtual bool							Reload() override;
	virtual std::vector<RenderElement*>*	GetRenderElements(int techniqueId) override;
	virtual int								GetTexId() override;
	virtual ITexture*						GetBaseTexture(int* passNum, int* nTU) override;
	virtual uint32							GetUsedTextureTypes() override;
	virtual ShaderTextureSlots*				GetUsedTextureSlots(int techniqueId) override;
	virtual int								GetTechniqueId(int techniqueIndex) override;
	virtual VertexFormat					GetVertexFormat() override { return m_vertexFormat; }
	// ~IShader Interface

	void									Free();

private:
	std::string								m_fileName;
	std::string								m_shaderName;
	ShaderType								m_shaderType;
	ShaderBlob								m_shaderBlob;
	CullType								m_cullType;
	uint32									m_flags;
	int										m_refreshFrameId;
	VertexFormat							m_vertexFormat;
	std::vector<ShaderTechnique*>			m_techniques;
	ShaderTextureSlots*						m_textureSlots[static_cast<int>(TechniqueId::Total)];

};

#endif //SHADER_H
