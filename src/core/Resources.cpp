#include "common.h"
#include "Resources.h"

Resources::Textures   Resources::textures_;
Resources::Animations Resources::animations_;

const rx::Texture &Resources::getTexture(int textureId)
{
	assert(textureId >= 0 && static_cast<std::size_t>(textureId) < textures_.size());

	return textures_[textureId];
}

const rx::Animation &Resources::getAnimation(int animationId)
{
	assert(animationId >= 0 && static_cast<std::size_t>(animationId) < animations_.size());

	return animations_[animationId];
}

void Resources::loadTextures(const char *path)
{
	String pathName(path);

	for (int i = 0; static_cast<std::size_t>(i) < textures_.size(); i++)
	{
		rx::Texture &tx = textures_[i];

		tx.data = new Texture();
		tx.data->setSmooth(true);

		if (!tx.data->loadFromFile(pathName + tx.fileName))
		{
			delete tx.data;
			tx.data = 0;
		}
	}
}

void Resources::releaseTextures()
{
	for (int i = 0; static_cast<std::size_t>(i) < textures_.size(); i++)
	{
		rx::Texture &tx = textures_[i];

		if (tx.data)
		{
			delete tx.data;
			tx.data = 0;
		}
	}
}
