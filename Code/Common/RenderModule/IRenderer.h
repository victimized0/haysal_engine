#ifndef INTERFACE_RENDERER_H
#define INTERFACE_RENDERER_H
#pragma once

#include <System\SystemInitParams.h>
#include "VertexFormats.h"
#include "IRenderView.h"

// Render features
#define FEATURE_OCCLUSION_QUERIES	0x4
#define FEATURE_HARDWARE_GAMMA		0x10
#define FEATURE_HDR					0x80000
#define FEATURE_SHADER_MODEL_4_0	0x0100000
#define FEATURE_SHADER_MODEL_5_0	0x0200000
#define FEATURE_VENDOR_INTEL		0x10000
#define FEATURE_VENDOR_AMD			0x20000
#define FEATURE_VENDOR_NVIDIA		0x40000
#define FEATURE_ORDER_RGBA			0x20000000

struct IShader;
struct IMaterial;
struct IRenderView;
struct IRenderNode;
struct IRenderMesh;
struct ShaderParam;
struct ShaderItem;
struct ShaderTechnique;
class  RenderMesh;

enum class RenderMeshType : uint8
{
	Immmutable = 0,
	Static = 1,
	Dynamic = 2,
};

enum RenderObjFlags : uint16
{
	TransformScale		= BIT16(0),
	TransformRotate		= BIT16(1),
	TransformTranslate	= BIT16(2),
	Selected			= BIT16(3),
	Decal				= BIT16(4),
	Indoors				= BIT16(5),
	Outdoors			= BIT16(6),
	LightVolume			= BIT16(7),
	Skinned				= BIT16(8),
	ZPrePass			= BIT16(9),
};

struct RenderParams
{
	RenderParams()
		: Alpha(1.f)
		, Flags(0)
		, pMatrix(nullptr)
		, pMaterial(nullptr)
		, pRenderNode(nullptr)
		, pInstance(nullptr)
		, pShaderParams(nullptr)
		, Distance(0.f)
	{}

	Matrix*						pMatrix;
	IMaterial*					pMaterial;
	IRenderNode*				pRenderNode;
	void*						pInstance;
	std::vector<ShaderParam>*	pShaderParams;
	Color						AmbientColor;
	float						Alpha;
	float						Distance;
	uint32						Flags;
};

struct RenderItem
{
	Matrix				WorldMatrix;
	IRenderMesh*		pRenderMesh;
	IRenderNode*		pRenderNode;
	ShaderTechnique*	pShaderTechnique;
	ShaderItem*			pShaderItem;
	//ShaderResources?
};

struct RenderPass
{
	std::vector<RenderItem>	RenderItems;
};

enum class RasterState : uint8
{
	Wireframe	= 0,
	Solid		= 1,
	Point		= 2
};

enum RenderFlags : uint16
{
	AllowHDR				= BIT32(0),
	AllowPostProcess		= BIT32(1),
	AllowRenderDebug		= BIT32(2),
	GenerateCubeMap         = BIT32(3),
	ZPass					= BIT32(4),
};

struct RenderViewport
{
	int X;
	int Y;
	int Width;
	int Height;
	float MinDepth;
	float MaxDepth;

	RenderViewport()
		: X(0)
		, Y(0)
		, Width(0)
		, Height(0)
		, MinDepth(0.f)
		, MaxDepth(1.f)
	{}

	RenderViewport(int x, int y, int w, int h)
		: X(x)
		, Y(y)
		, Width(w)
		, Height(h)
		, MinDepth(0.f)
		, MaxDepth(1.f)
	{}
};

struct IRenderer
{
	virtual					~IRenderer() {}

	virtual WIN_HWND		Init(int width, int height, const SystemInitParams& initParams) = 0;
	virtual void			PostInit() = 0;
	virtual void			Release() = 0;
	virtual void			ShutDown() = 0;

	virtual void			BeginFrame() = 0;
	virtual void			EndFrame() = 0;
	virtual void			RenderScene(IRenderView* renderView) = 0;

	//virtual void			InitResources(int flags) = 0;
	//virtual void			FreeResources(int flags) = 0;
	virtual void			InitSysResources() = 0;
	virtual void			FreeSysResources(int flags) = 0;

	virtual IDXGISurface*	GetBackBuffer(struct GpuResource** pRes) const = 0;

	virtual int				GetHeight() const = 0;
	virtual int				GetWidth() const = 0;

	//virtual IShader*		LoadShader(const char* name, int flags = 0, uint64 nMaskGen = 0) = 0;
	//virtual ITexture*		GetTextureByID(int Id) = 0;
	//virtual ITexture*		GetTextureByName(const char* name, uint32 flags = 0) = 0;
	//virtual ITexture*		LoadTexture(const char* nameTex, const uint32 flags = 0) = 0;

	//virtual ITexture*		CreateTexture(const char* name, int width, int height, int numMips, unsigned char* pData, ETEX_Format eTF, int flags) = 0;
	//virtual ITexture*		CreateTextureArray(const char* name, ETEX_Type eType, uint32 nWidth, uint32 nHeight, uint32 nArraySize, int nMips, uint32 nFlags, ETEX_Format eTF, int nCustomID) = 0;

	virtual IRenderMesh*	CreateRenderMesh(const char* srcName, RenderMeshType bufferType = RenderMeshType::Static) = 0;

	virtual void			FlushRenderList() = 0;
	virtual void			PushProfileMarker(const char* label) = 0;
	virtual void			PopProfileMarker(const char* label) = 0;

	virtual int				GetFrameID() = 0;
	virtual WIN_HWND		GetHWND() = 0;
	virtual void			SetHWND(WIN_HWND hWnd) = 0;

	virtual IRenderView*	AllocateRenderView() const = 0;
};

#endif //INTERFACE_RENDERER_H
