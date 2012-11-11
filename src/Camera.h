#pragma once

class Camera : public Entity
{
	public:

		Camera() : kMinVel(20.0f), objective_(0) {}

		void update(float dt);

		void moveToObjective();

		void setObjective(Entity *objective);

		Entity *getObjective();

		void setBounds(const Vector2f &bounds);

		void updateViewSize(const Vector2u &viewSize);

		const sf::View &getView();

	private:

		const float kMinVel;

		sf::View view_;
		Vector2f bounds_;

		Entity *objective_;
};
