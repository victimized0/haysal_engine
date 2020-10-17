#ifndef INTERFACE_RENDERER_H
#define INTERFACE_RENDERER_H
#pragma once

#include <System\SystemInitParams.h>

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

struct IRenderNode;

struct RenderInfo
{

};

enum class RasterState : byte
{
	Wireframe	= 0,
	Solid		= 1,
	Point		= 2
};

struct IRenderer
{
	virtual				~IRenderer() {}

	virtual WIN_HWND	Init(int width, int height, const SystemInitParams& initParams) = 0;
	virtual void		PostInit() = 0;
	
	virtual void		Release() = 0;
	virtual void		ShutDown() = 0;

	virtual void		BeginFrame() = 0;
	virtual void		FillFrame(float clearColor) = 0;
	virtual void		EndFrame() = 0;

	//virtual void		InitResources(int flags) = 0;
	//virtual void		FreeResources(int flags) = 0;

	virtual int			GetHeight() const = 0;
	virtual int			GetWidth() const = 0;

	//virtual IShader*	LoadShader(const char* name, int flags = 0, uint64 nMaskGen = 0) = 0;
	//virtual ITexture*	GetTextureByID(int Id) = 0;
	//virtual ITexture*	GetTextureByName(const char* name, uint32 flags = 0) = 0;
	//virtual ITexture*	LoadTexture(const char* nameTex, const uint32 flags = 0) = 0;

	//virtual ITexture* CreateTexture(const char* name, int width, int height, int numMips, unsigned char* pData, ETEX_Format eTF, int flags) = 0;
	//virtual ITexture* CreateTextureArray(const char* name, ETEX_Type eType, uint32 nWidth, uint32 nHeight, uint32 nArraySize, int nMips, uint32 nFlags, ETEX_Format eTF, int nCustomID) = 0;

	virtual void		FlushRenderList() = 0;
	virtual void		AddRenderItem(const RenderInfo& info) = 0;

	virtual void		PushProfileMarker(const char* label) = 0;
	virtual void		PopProfileMarker(const char* label) = 0;

	virtual int			GetFrameID() = 0;
	virtual WIN_HWND	GetHWND() = 0;
};

#endif //INTERFACE_RENDERER_H
