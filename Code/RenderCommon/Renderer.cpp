#include "stdafx.h"
#include "Renderer.h"

Renderer* gRenderer = nullptr;

Renderer::Renderer()
	: m_frameID(0)
{

}

void Renderer::Release()
{
	gRenderer = nullptr;
}

void Renderer::FlushRenderList()
{

}

void Renderer::AddRenderItem(const RenderInfo& info)
{

}

void Renderer::InitRenderer()
{
	if (!gRenderer)
		gRenderer = this;
}

int Renderer::GetFrameID()
{
	return m_frameID;
}
