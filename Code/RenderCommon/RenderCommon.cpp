#include "StdAfx.h"
#include "RenderCommon.h"

std::map<std::string, BaseResource*>	BaseResource::s_resourcesMap;
std::vector<BaseResource*>				BaseResource::s_resources;
std::vector<int>						BaseResource::s_freeIndices;

int BaseResource::Release()
{
	if (m_refCount <= 1)
	{
		Unregister();
	}
	else
	{
		assert(false);
	}

	return m_refCount;
}

void BaseResource::Register(const std::string& name)
{
	s_resourcesMap.insert({ name, this });

	int index;
	if (s_freeIndices.size() > 0)
	{
		// Reuse free spot instead of allocating new memory
		auto it = s_freeIndices.end() - 1;
		index = *it;
		s_freeIndices.erase(it);
		s_resources[index] = this;
	}
	else
	{
		// Otherwise push a new item
		index = static_cast<int>(s_resources.size());
		s_resources.push_back(this);
	}

	m_id		= index;
	m_name		= name;
	m_refCount	= 1;
}

void BaseResource::Unregister()
{
	if (IsValid())
	{
		s_resourcesMap.erase(m_name);
		s_resources[m_id] = nullptr;
		s_freeIndices.push_back(m_id);
		delete this;
	}
}

bool BaseResource::IsValid()
{
	if (s_resourcesMap.find(m_name) == s_resourcesMap.end())
	{
		return false;
	}

	return m_id >= 0 && m_id < s_resources.size();
}

void BaseResource::ShutDown()
{
	for (auto it = s_resourcesMap.begin(); it != s_resourcesMap.end(); /**/)
	{
		BaseResource* pRes = it->second;
		++it;
		SAFE_RELEASE(pRes);
	}

	s_resourcesMap.clear();
	s_resources.clear();
	s_freeIndices.clear();
}

BaseResource* BaseResource::Get(int id, bool addRef)
{
	if (id >= (int)s_resources.size() || id < 0)
		return nullptr;

	BaseResource* pResource = s_resources[id];
	if (pResource && addRef)
		pResource->AddRef();

	return pResource;
}

BaseResource* BaseResource::Get(const std::string& name, bool addRef)
{
	auto it = s_resourcesMap.find(name);
	if (it != s_resourcesMap.end())
	{
		BaseResource* pResource = it->second;
		if (pResource && addRef)
			pResource->AddRef();
		return pResource;
	}
	return nullptr;
}

ResourceView ResourceView::ShaderResourceView(DXGIFormat format, int firstElementNum, int elementsCount, int mostDetailedMipNum, int mipsCount, bool isSrgbRead, bool isMultisampled, uint32 flags)
{
	ResourceView result(0);

	result.Desc.ViewType			= ResourceView::Type::ShaderResourceView;
	result.Desc.Format				= format;
	result.Desc.Flags				= flags;
	result.Desc.FirstSliceNum		= firstElementNum;
	result.Desc.SlicesCount			= elementsCount;
	result.Desc.MostDetailedMipNum	= mostDetailedMipNum;
	result.Desc.MipsCount			= mipsCount;
	result.Desc.IsSrgbRead			= isSrgbRead		? 1 : 0;
	result.Desc.IsMultisampled		= isMultisampled	? 1 : 0;

	return result;
}

ResourceView ResourceView::RenderTargetView(DXGIFormat format, int firstElementNum, int elementsCount, int mipLevel, bool isMultisampled)
{
	ResourceView result(0);

	result.Desc.ViewType			= ResourceView::Type::RenderTargetView;
	result.Desc.Format				= format;
	result.Desc.FirstSliceNum		= firstElementNum;
	result.Desc.SlicesCount			= elementsCount;
	result.Desc.MostDetailedMipNum	= mipLevel;
	result.Desc.MipsCount			= 1;
	result.Desc.IsMultisampled		= isMultisampled ? 1 : 0;

	return result;
}

ResourceView ResourceView::DepthStencilView(DXGIFormat format, int firstElementNum, int elementsCount, int mipLevel, bool isMultisampled, uint32 flags)
{
	ResourceView result(0);

	result.Desc.ViewType			= ResourceView::Type::DepthStencilView;
	result.Desc.Format				= format;
	result.Desc.Flags				= flags;
	result.Desc.FirstSliceNum		= firstElementNum;
	result.Desc.SlicesCount			= elementsCount;
	result.Desc.MostDetailedMipNum	= mipLevel;
	result.Desc.MipsCount			= 1;
	result.Desc.IsMultisampled		= isMultisampled ? 1 : 0;

	return result;
}

ResourceView ResourceView::UnorderedAccessView(DXGIFormat format, int firstElementNum, int elementsCount, int mipLevel, uint32 flags)
{
	ResourceView result(0);

	result.Desc.ViewType			= ResourceView::Type::UnorderedAccessView;
	result.Desc.Format				= format;
	result.Desc.Flags				= flags;
	result.Desc.FirstSliceNum		= firstElementNum;
	result.Desc.SlicesCount			= elementsCount;
	result.Desc.MostDetailedMipNum	= mipLevel;
	result.Desc.MipsCount			= 1;

	return result;
}

//ResourceView ResourceView::ShaderResourceRawView(DXGIFormat format, int firstElementNum, int elementsCount, uint32 flags)
//{
//	ResourceView result(0);
//
//	result.Desc.ViewType		= ResourceView::Type::ShaderResourceView;
//	result.Desc.Format			= format;
//	result.Desc.OffsetBitsCount = IntegerLog2_RoundUp(uint32(firstElementNum + 1));
//	result.Desc.OffsetAndSize	= uint64(firstElementNum) << (46 - result.Desc.OffsetBitsCount);
//	result.Desc.OffsetAndSize	|= uint64(elementsCount) & MASK64(46 - result.Desc.OffsetBitsCount);
//	result.Desc.Flags			= flags;
//
//	return result;
//}
//
//ResourceView ResourceView::RenderTargetRawView(DXGIFormat format, int firstElementNum, int elementsCount)
//{
//	return ResourceView();
//}
//
//ResourceView ResourceView::DepthStencilRawView(DXGIFormat format, int firstElementNum, int elementsCount, uint32 flags)
//{
//	return ResourceView();
//}
//
//ResourceView ResourceView::UnorderedAccessRawView(DXGIFormat format, int firstElementNum, int elementsCount, uint32 flags)
//{
//	return ResourceView();
//}

