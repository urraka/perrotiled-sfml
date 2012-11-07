#pragma once

class Player : public Entity
{
	public:

		Player() { textureId_ = rx::kPerro; position_ = Vector2f(400.0f, 300.0f); }

		void update(float dt);

		void setDrawOffset(const Vector2f &offset);
};
