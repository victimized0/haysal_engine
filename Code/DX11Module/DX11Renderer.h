#ifndef DIRECTX11_RENDER_MODULE_H
#define DIRECTX11_RENDER_MODULE_H
#pragma once

#include <RenderModule\IRenderModule.h>

class DX11RendModule : public IRenderModule
{
public:
	virtual			~DX11RendModule();

	const char*		GetName() const final;
	bool			Initialize(Environment& env, const SystemInitParams& initParams) final;
};

#endif //DIRECTX11_RENDER_MODULE_H
