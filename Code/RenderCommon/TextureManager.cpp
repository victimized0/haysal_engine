#include "StdAfx.h"
#include "TextureManager.h"

TextureManager::~TextureManager()
{
	ReleaseDefaultTextures();
}

void TextureManager::PreloadDefaultTextures()
{
	if (m_defaultTextures.size())
		return;

	pugi::xml_document doc = gEnv->pSystem->LoadXmlFromFile("Engine/default_textures.xml");
	const pugi::xml_node& root = doc.child("Textures");
	for (pugi::xml_node entry = root.first_child(); entry; entry = entry.next_sibling())
	{
		if (std::string(entry.name()) != "Texture")
			continue;

		uint32 flags = 0;

		// check attributes to modify the loading flags
		bool noMips = entry.attribute("NoMips").as_bool(false);
		if (noMips) flags |= TextureFlags::NoMips;

		const char* path = entry.attribute("Path").as_string("");
		Texture* pTexture = Texture::ForName(path, flags, TextureFormat::Unknown);
		if (pTexture)
		{
			const std::string name = Path::GetNameWithoutExt(path);
			m_defaultTextures[name] = pTexture;
		}
	}
}

void TextureManager::ReleaseDefaultTextures()
{
	for (auto it = m_defaultTextures.begin(); it != m_defaultTextures.end(); ++it)
		SAFE_RELEASE(it->second);
	m_defaultTextures.clear();
}

const Texture* TextureManager::GetDefaultTexture(const std::string& name) const
{
	auto it = m_defaultTextures.find(name);
	if (it == m_defaultTextures.end())
		return nullptr;
	return it->second;
}
