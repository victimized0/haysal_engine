#ifndef INTERFACE_RENDERER_H
#define INTERFACE_RENDERER_H
#pragma once

#include "System\SystemInitParams.h"

struct RenderInfo
{

};

struct Material
{

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

	//virtual void		AddDynLight(RenderLight* Source, const RenderingPassInfo& passInfo) = 0;
	//virtual bool		UpdateDynLight(SRenderLight* pDL) const = 0;
	//virtual int		AddDeferredLight(const RenderLight& pLight, float fMult, const RenderingPassInfo& passInfo) = 0;

	//virtual int		GetFrameID() = 0;

	virtual void		PushProfileMarker(const char* label) = 0;
	virtual void		PopProfileMarker(const char* label) = 0;

	virtual WIN_HWND	GetHWND() = 0;
};

#endif //INTERFACE_RENDERER_H
