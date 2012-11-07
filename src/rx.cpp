#include "rx.h"
#include "core/SFMLGame.h"

namespace rx
{
	void initialize()
	{
		// Resources initialization

		Resources::Textures &textures = Resources::textures_;
		Resources::Animations &animations = Resources::animations_;
		Animation::Frames frames;

		textures.resize(kTextureCount);
		animations.resize(kAnimationCount);

		textures[kGroundTexture] = Texture("maps/map_ground.png", Vector2i(0,0), Vector2u(32,32));
		textures[kRuby] = Texture("gfx/ruby.png", Vector2i(26,78), Vector2u(52,80));
		textures[kPerro] = Texture("gfx/perro.png", Vector2i(26,78), Vector2u(52,80));

		animations[kWalking].frames.push_back(Frame(-1, 0.10f, 1));
		animations[kWalking].frames.push_back(Frame(-1, 0.10f, 2));
	}
}
