#ifndef RENDERER_VK_IMPL_INL
#define RENDERER_VK_IMPL_INL
#pragma once
#error Not implemented

inline D3D11_BIND_FLAG ConvertBindFlags(uint32 flags)
{
	return 0;
}

inline D3D11_CPU_ACCESS_FLAG ConvertCPUAccessFlags(uint32 flags)
{
	return 0;
}

inline D3D11_USAGE ConvertUsage(uint32 flags)
{
	return 0;
}

inline D3D11_RESOURCE_MISC_FLAG ConvertMiscFlags(uint32 flags)
{
	return 0;
}

template<typename T>
static inline uint32 ToIntermediateFlags(const T& flags)
{
	return 0;
}

#endif //RENDERER_VK_IMPL_INL
