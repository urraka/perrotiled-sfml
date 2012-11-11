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
		void respawn(Map *map);
		void keyPressed(const sf::Event::KeyEvent &keyEvent);

		FloatRect getBounds();

	private:

		struct Action
		{
			Action()
				:	goLeft(false),
					goRight(false),
					jump(false),
					kick(false) {}

			bool goLeft;
			bool goRight;
			bool jump;
			bool kick;
		};

		Action processInput();

		Controls controls_;

		Vector2f acceleration_;
		MapCollision mapCollision_;

		Animation animation_;
};
