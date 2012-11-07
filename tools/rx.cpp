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

		/*--INITIALIZE--*/
	}
}
