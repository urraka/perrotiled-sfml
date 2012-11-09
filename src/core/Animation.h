#pragma once

class Animation
{
	public:

		Animation();

		void update(float dt);

		bool is(int animationId);
		void reset();
		void set(int animationId, int startFrame = 0);
		int  get();

		int getCurrentTexture();
		int getCurrentImageIndex();

	private:

		int id_;
		int frame_;
		float time_;
};
