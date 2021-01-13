#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#pragma once

class Texture;

class TextureManager
{
public:
					TextureManager() {}
	virtual 		~TextureManager();

	void            PreloadDefaultTextures();
	void            ReleaseDefaultTextures();

	const Texture*	GetDefaultTexture(const std::string& name) const;

private:
	std::map<std::string, Texture*> m_defaultTextures;
};

#endif //TEXTURE_MANAGER_H
