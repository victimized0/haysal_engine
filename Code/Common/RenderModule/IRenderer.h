#ifndef INTERFACE_RENDERER_H
#define INTERFACE_RENDERER_H
#pragma once

#include "../SystemInitParams.h"

struct IRenderer
{
	virtual				~IRenderer() {}

	virtual PHWND		Init(int x, int y, int width, int height, struct SystemInitParams& initParams) = 0;
	virtual void		PostInit() = 0;
	
	virtual int			GetFeatures() = 0;
	virtual void		Release() = 0;
	virtual void		ShutDown() = 0;

	virtual void		BeginFrame() = 0;
	virtual void		FillFrame(float clearColor) = 0;
	virtual void		EndFrame() = 0;

	virtual void		InitSystemResources(int nFlags) = 0;
	virtual void		FreeSystemResources(int nFlags) = 0;

	virtual int			GetHeight() const = 0;
	virtual int			GetWidth() const = 0;
	virtual float		GetPixelAspectRatio() const = 0;

	//virtual IShader*	LoadShader(const char* name, int flags = 0, uint64 nMaskGen = 0) = 0;
	//virtual ITexture*	GetTextureByID(int Id) = 0;
	//virtual ITexture*	GetTextureByName(const char* name, uint32 flags = 0) = 0;
	//virtual ITexture*	LoadTexture(const char* nameTex, const uint32 flags = 0) = 0;

	//virtual ITexture* CreateTexture(const char* name, int width, int height, int numMips, unsigned char* pData, ETEX_Format eTF, int flags) = 0;
	//virtual ITexture* CreateTextureArray(const char* name, ETEX_Type eType, uint32 nWidth, uint32 nHeight, uint32 nArraySize, int nMips, uint32 nFlags, ETEX_Format eTF, int nCustomID) = 0;

	//virtual void		ADDDlight(SRenderLight* Source, const SRenderingPassInfo& passInfo) = 0;
	//virtual bool		UpdateDLight(SRenderLight* pDL) const = 0;
	//virtual int		AddDeferredLight(const SRenderLight& pLight, float fMult, const SRenderingPassInfo& passInfo) = 0;

	//virtual int			GetFrameID(bool bIncludeRecursiveCalls = true) = 0;

	virtual void		PushProfileMarker(const char* label) = 0;
	virtual void		PopProfileMarker(const char* label) = 0;

	virtual PHWND		GetHWND() = 0;
	virtual bool        SetWindowIcon(const char* path) = 0;
};

#endif //INTERFACE_RENDERER_H
