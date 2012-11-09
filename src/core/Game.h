#pragma once

class Game
{
	public:

		Game::Game();

		virtual Game::~Game();

		void setWindowless(bool isWindowless);

		void setName(const char *name);

		void setVideoMode(VideoMode videoMode, Uint32 style = sf::Style::Default);

		void setVerticalSync(bool enabled);

		bool isVerticalSyncEnabled();

		void setTickRate(int tickrate);

		void start(int argc = 0, char **argv = 0);

		void quit();

		int  getFps();

		void interpolatePosition(Entity &entity);

		RenderWindow &getRenderWindow();

	protected:

		virtual void initialize(int argc = 0, char **argv = 0) = 0;
		virtual void update(float dt) = 0;
		virtual void draw(RenderTarget &renderTarget) = 0;

		// events
		virtual void windowResized(const Vector2u &size) {}
		virtual void keyPressed(const sf::Event::KeyEvent &keyEvent) {};
		virtual void keyReleased(const sf::Event::KeyEvent &keyEvent) {};

	private:

		Uint32 windowStyle_;
		String name_;
		VideoMode videoMode_;
		RenderWindow *mainWindow_;

		Time dt_;

		int  fps_;
		bool started_;
		bool quit_;
		bool isWindowless_;
		bool isVerticalSyncEnabled_;

		float interpolateValue_;
};
