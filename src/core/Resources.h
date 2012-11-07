#pragma once

namespace rx
{
	void initialize();

	struct Texture
	{
		String fileName;
		Vector2i origin;
		Vector2u size;
		::Texture *data;

		Texture() : data(0) {}

		Texture(const char *file, const Vector2i &orig, const Vector2u &sz)
			:	fileName(file),
				origin(orig),
				size(sz),
				data(0) {}
	};

	struct Frame
	{
		int textureId;
		int imageIndex;
		float duration;

		Frame(int txId, float d, int imgIdx = 0)
			:	textureId(txId),
				imageIndex(imgIdx),
				duration(d) {}
	};

	struct Animation
	{
		typedef std::vector<Frame> Frames;

		Frames frames;
	};
}

class Resources
{
	public:

		static const rx::Texture   &getTexture(int textureId);
		static const rx::Animation &getAnimation(int animationId);

		static void loadTextures(const char *path);
		static void releaseTextures();

	private:

		typedef std::vector<rx::Texture>   Textures;
		typedef std::vector<rx::Animation> Animations;

		static Textures   textures_;
		static Animations animations_;

	friend void rx::initialize();
};
