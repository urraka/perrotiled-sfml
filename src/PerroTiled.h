#pragma once

class PerroTiled : public Game
{
	public:

		PerroTiled() : background(FloatRect(), Color::Blue, Color::White, LinearGradient::kVertical), worldTexture(0) {}

		~PerroTiled();

		void initialize(int argc = 0, char **argv = 0);

		void update(float dt);

		void draw(RenderTarget &renderTarget);

		void onWindowResize(const Vector2u &size);

	private:

		Vector2f getViewSize();

		Map map;
		Camera camera;
		Player player;
		LinearGradient background;

		sf::RenderTexture *worldTexture;
		sf::Sprite worldSprite;
};
