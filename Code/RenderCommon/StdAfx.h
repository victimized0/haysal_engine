#ifndef RENDERER_STDAFX_H
#define RENDERER_STDAFX_H
#pragma once

#include <platform.h>

#if PLATFORM_WINDOWS
	#include <d3d11_3.h>

	#include <d3d11sdklayers.h>
	#include <d3d11shader.h>
	#include <d3dcompiler.h>

	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "d3dcompiler.lib")

#elif PLATFORM_DURANGO
	#include <d3d11_x.h>
	#include <d3d11shader_x.h>
	#include <d3dcompiler_x.h>
#endif // PLATFORM

#endif //RENDERER_STDAFX_H
