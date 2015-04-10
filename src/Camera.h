#pragma once

class Camera : public Entity
{
	public:

		Camera()
			:	objective_(0),
				changingObjectiveX_(false),
				changingObjectiveY_(false) {}

		void update(float dt);
		void update2(float dt);

		void moveToObjective();

		void setObjective(Entity *objective);

		Entity *getObjective();

		void setBounds(const Vector2f &bounds);

		void updateViewSize(const Vector2u &viewSize);

		const sf::View &getView();

	private:

		Entity *objective_;

		Vector2f acceleration_;

		sf::View view_;
		Vector2f bounds_;

		bool changingObjectiveX_;
		bool changingObjectiveY_;
};
