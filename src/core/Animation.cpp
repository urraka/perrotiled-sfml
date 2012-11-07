#include "common.h"
#include "Resources.h"
#include "Animation.h"

Animation::Animation() :
	id_(-1),
	frame_(0),
	time_(0.0f)
{
}

void Animation::update(float dt)
{
	if (id_ != -1)
	{
		time_ += dt;

		const rx::Animation &anim = Resources::getAnimation(id_);

		while (time_ > anim.frames[frame_].duration)
		{
			time_ -= anim.frames[frame_].duration;
			
			if (++frame_ == anim.frames.size())
			{
				frame_ = 0;
			}
		}
	}
}

bool Animation::is(int animationId)
{
	return animationId == id_;
}

void Animation::reset()
{
	frame_ = 0;
	time_ = 0.0f;
}

void Animation::set(int animationId)
{
	id_ = animationId;
	frame_ = 0;
	time_ = 0.0f;
}

int Animation::get()
{
	return id_;
}

int Animation::getCurrentTexture()
{
	assert(id_ != -1 && frame_ >= 0 && static_cast<std::size_t>(frame_) < Resources::getAnimation(id_).frames.size());

	return Resources::getAnimation(id_).frames[frame_].textureId;
}

int Animation::getCurrentImageIndex()
{
	assert(id_ != -1 && frame_ >= 0 && static_cast<std::size_t>(frame_) < Resources::getAnimation(id_).frames.size());

	return Resources::getAnimation(id_).frames[frame_].imageIndex;
}
