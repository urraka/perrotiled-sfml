#pragma once

class Camera : public Entity
{
	public:

		Camera()
			:	kMinVel(20.0f),
				objective_(0),
				changingObjectiveX_(false),
				changingObjectiveY_(false) {}

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
		bool changingObjectiveX_;
		bool changingObjectiveY_;
};
