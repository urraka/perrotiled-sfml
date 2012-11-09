#pragma once

class Map;

class Player : public Entity
{
	public:

		struct Controls
		{
			Keyboard::Key left;
			Keyboard::Key right;
			Keyboard::Key jump;
		};

		void init(int texture, const Controls &controls, Map *map);
		void update(float dt);

		FloatRect getBounds();

	private:

		struct Action
		{
			Action()
				:	goLeft(false),
					goRight(false),
					jump(false) {}

			bool goLeft;
			bool goRight;
			bool jump;
		};

		Action processInput();

		Controls controls_;

		Vector2f acceleration_;
		MapCollision mapCollision_;

		Animation animation_;
};
