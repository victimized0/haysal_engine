#ifndef RENDERER_DX11_IMPL_INL
#define RENDERER_DX11_IMPL_INL
#pragma once

inline D3D11_BIND_FLAG ConvertBindFlags(uint32 flags)
{
	assert((flags & (ResourceFlags::BIND_RENDER_TARGET | ResourceFlags::BIND_DEPTH_STENCIL)) != (ResourceFlags::BIND_RENDER_TARGET | ResourceFlags::BIND_DEPTH_STENCIL));

	return D3D11_BIND_FLAG (
		((flags & ResourceFlags::BIND_VERTEX_BUFFER)	? D3D11_BIND_VERTEX_BUFFER		: 0) |
		((flags & ResourceFlags::BIND_INDEX_BUFFER)		? D3D11_BIND_INDEX_BUFFER		: 0) |
		((flags & ResourceFlags::BIND_CONST_BUFFER)		? D3D11_BIND_CONSTANT_BUFFER	: 0) |
		((flags & ResourceFlags::BIND_SHADER_RESOURCE)	? D3D11_BIND_SHADER_RESOURCE	: 0) |
		((flags & ResourceFlags::BIND_RENDER_TARGET)	? D3D11_BIND_RENDER_TARGET		: 0) |
		((flags & ResourceFlags::BIND_DEPTH_STENCIL)	? D3D11_BIND_DEPTH_STENCIL		: 0) |
		((flags & ResourceFlags::BIND_UNORDERED_ACCESS) ? D3D11_BIND_UNORDERED_ACCESS	: 0) );
}

inline D3D11_CPU_ACCESS_FLAG ConvertCPUAccessFlags(uint32 flags)
{
	return D3D11_CPU_ACCESS_FLAG (
		((flags & ResourceFlags::USAGE_CPU_WRITE) ? D3D11_CPU_ACCESS_WRITE : 0) |
		((flags & ResourceFlags::USAGE_CPU_READ ) ? D3D11_CPU_ACCESS_READ  : 0) );
}

inline D3D11_USAGE ConvertUsageFlags(uint32 flags)
{
	assert((flags & (ResourceFlags::USAGE_CPU_READ | ResourceFlags::USAGE_CPU_WRITE)) != (ResourceFlags::USAGE_CPU_READ | ResourceFlags::USAGE_CPU_WRITE));

	return D3D11_USAGE (
		((flags & ResourceFlags::USAGE_CPU_READ)	? D3D11_USAGE_DEFAULT : //D3D11_USAGE_STAGING :
		((flags & ResourceFlags::USAGE_CPU_WRITE)	? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT)));
}

inline D3D11_RESOURCE_MISC_FLAG ConvertMiscFlags(uint32 flags)
{
	return D3D11_RESOURCE_MISC_FLAG (
		((flags & ResourceFlags::USAGE_AUTOGENMIPS)		? D3D11_RESOURCE_MISC_GENERATE_MIPS          : 0) |
		((flags & ResourceFlags::USAGE_STRUCTURED)		? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED      : 0) |
		((flags & ResourceFlags::USAGE_RAW)				? D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS : 0) |
		((flags & ResourceFlags::USAGE_INDIRECTARGS)	? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS      : 0) );
}

template<typename T>
static inline uint32 ToIntermediateFlags(const T& flags)
{
	return
		((flags.BindFlags		& D3D11_BIND_VERTEX_BUFFER)					? ResourceFlags::BIND_VERTEX_BUFFER		: 0) |
		((flags.BindFlags		& D3D11_BIND_INDEX_BUFFER)					? ResourceFlags::BIND_INDEX_BUFFER		: 0) |
		((flags.BindFlags		& D3D11_BIND_CONSTANT_BUFFER)				? ResourceFlags::BIND_CONST_BUFFER		: 0) |
		((flags.BindFlags		& D3D11_BIND_SHADER_RESOURCE)				? ResourceFlags::BIND_SHADER_RESOURCE	: 0) |
		((flags.BindFlags		& D3D11_BIND_RENDER_TARGET)					? ResourceFlags::BIND_RENDER_TARGET		: 0) |
		((flags.BindFlags		& D3D11_BIND_DEPTH_STENCIL)					? ResourceFlags::BIND_DEPTH_STENCIL		: 0) |
		((flags.BindFlags		& D3D11_BIND_UNORDERED_ACCESS)				? ResourceFlags::BIND_UNORDERED_ACCESS	: 0) |
		((flags.CPUAccessFlags	& D3D11_CPU_ACCESS_WRITE)					? ResourceFlags::USAGE_CPU_WRITE		: 0) |
		((flags.CPUAccessFlags	& D3D11_CPU_ACCESS_READ)					? ResourceFlags::USAGE_CPU_READ			: 0) |
		((flags.MiscFlags		& D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)	? ResourceFlags::USAGE_STRUCTURED		: 0) |
		((flags.MiscFlags		& D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS)	? ResourceFlags::USAGE_INDIRECTARGS		: 0) |
		((flags.Usage			== D3D11_USAGE_DYNAMIC)						? ResourceFlags::USAGE_CPU_WRITE		: 0) |
		((flags.Usage			== D3D11_USAGE_STAGING)						? ResourceFlags::USAGE_CPU_READ			: 0);
}

#endif //RENDERER_DX11_IMPL_INL
