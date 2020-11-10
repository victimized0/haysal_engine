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

ResourceView ResourceView::ShaderResource(DXGIFormat format, int flags)
{
	ResourceView result;

	result.Type				= ResourceView::Type::ShaderResourceView;
	result.Format			= format;
	result.Flags			= flags;
	//result.FirstSlice		= firstSlice;
	//result.SliceCount		= slicesCount;
	//result.MostDetailedMip	= mostDetailedMipNum;
	//result.MipCount			= mipsCount;
	//result.SrgbRead			= isSrgbRead ? 1 : 0;
	//result.Multisample		= isMSAA ? 1 : 0;

	return result;
}

ResourceView ResourceView::RenderTarget(DXGIFormat format)
{
	ResourceView result;

	result.Type					= ResourceView::Type::RenderTargetView;
	result.Format				= format;
	//result.nFirstSlice		= nFirstSlice;
	//result.nSliceCount		= nSliceCount;
	//result.nMostDetailedMip	= nMipLevel;
	//result.nMipCount			= 1;
	//result.bMultisample		= bMultisample ? 1 : 0;

	return result;
}

ResourceView ResourceView::DepthStencil(DXGIFormat format, int flags)
{
	ResourceView result;

	result.Type					= ResourceView::Type::DepthStencilView;
	result.Format				= format;
	result.Flags				= flags;
	//result.nFirstSlice		= nFirstSlice;
	//result.nSliceCount		= nSliceCount;
	//result.nMostDetailedMip	= nMipLevel;
	//result.nMipCount			= 1;
	//result.bMultisample		= bMultisample ? 1 : 0;

	return result;
}

ResourceView ResourceView::UnorderedAccess(DXGIFormat format, int flags)
{
	ResourceView result;

	result.Type					= ResourceView::Type::UnorderedAccessView;
	result.Format				= format;
	result.Flags				= flags;
	//result.FirstSlice			= nFirstSlice;
	//result.SliceCount			= nSliceCount;
	//result.MostDetailedMip	= nMipLevel;
	//result.MipCount			= 1;

	return result;
}

//ResourceView ResourceView::ShaderResourceRawView(DXGIFormat format, int flags)
//{
//	ResourceView result;
//
//	result.Type				= ResourceView::Type::ShaderResourceView;
//	result.Format			= format;
//	result.Flags			= flags;
//	//result.OffsetBits		= IntegerLog2_RoundUp(uint32(nFirstElement + 1));
//	//result.OffsetAndSize	= uint64(nFirstElement) << (46 - result.m_Desc.nOffsetBits);
//	//result.OffsetAndSize	|= uint64(nElementCount) & MASK64(46 - result.m_Desc.nOffsetBits);
//
//	//assert(nFirstElement == UINT(result.m_Desc.nOffsetAndSize >> (46 - result.m_Desc.nOffsetBits)));
//	//assert(nElementCount == UINT(result.m_Desc.nOffsetAndSize & MASK64(46 - result.m_Desc.nOffsetBits)));
//
//	return result;
//}
//
//ResourceView ResourceView::RenderTargetRawView(DXGIFormat format)
//{
//	ResourceView result;
//
//	result.Type				= ResourceView::Type::RenderTargetView;
//	result.Format			= format;
//	//result.OffsetBits		= IntegerLog2_RoundUp(uint32(nFirstElement + 1));
//	//result.OffsetAndSize	= uint64(nFirstElement) << (46 - result.m_Desc.nOffsetBits);
//	//result.OffsetAndSize	|= uint64(nElementCount) & MASK64(46 - result.m_Desc.nOffsetBits);
//
//	//assert(nFirstElement == UINT(result.m_Desc.nOffsetAndSize >> (46 - result.m_Desc.nOffsetBits)));
//	//assert(nElementCount == UINT(result.m_Desc.nOffsetAndSize & MASK64(46 - result.m_Desc.nOffsetBits)));
//
//	return result;
//}
//
//ResourceView ResourceView::DepthStencilRawView(DXGIFormat format, int flags)
//{
//	ResourceView result;
//
//	result.Type				= ResourceView::Type::DepthStencilView;
//	result.Format			= format;
//	result.Flags			= flags;
//	//result.OffsetBits		= IntegerLog2_RoundUp(uint32(nFirstElement + 1));
//	//result.OffsetAndSize	= uint64(nFirstElement) << (46 - result.m_Desc.nOffsetBits);
//	//result.OffsetAndSize	|= uint64(nElementCount) & MASK64(46 - result.m_Desc.nOffsetBits);
//
//	//assert(nFirstElement == UINT(result.m_Desc.nOffsetAndSize >> (46 - result.m_Desc.nOffsetBits)));
//	//assert(nElementCount == UINT(result.m_Desc.nOffsetAndSize & MASK64(46 - result.m_Desc.nOffsetBits)));
//
//	return result;
//}
//
//ResourceView ResourceView::UnorderedAccessRawView(DXGIFormat format, int flags)
//{
//	ResourceView result;
//
//	result.Type				= ResourceView::Type::UnorderedAccessView;
//	result.Format			= format;
//	result.Flags			= flags;
//	//result.OffsetBits		= IntegerLog2_RoundUp(uint32(nFirstElement + 1));
//	//result.OffsetAndSize	= uint64(nFirstElement) << (46 - result.m_Desc.nOffsetBits);
//	//result.OffsetAndSize	|= uint64(nElementCount) & MASK64(46 - result.m_Desc.nOffsetBits);
//
//	//assert(nFirstElement == UINT(result.m_Desc.nOffsetAndSize >> (46 - result.m_Desc.nOffsetBits)));
//	//assert(nElementCount == UINT(result.m_Desc.nOffsetAndSize & MASK64(46 - result.m_Desc.nOffsetBits)));
//
//	return result;
//}
