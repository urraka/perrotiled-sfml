#include "common.h"
#include "Animation.h"
#include "Entity.h"
#include "Game.h"

Game::Game()
	:	mainWindow_(0),
		fps_(0),
		started_(false),
		quit_(false),
		isVerticalSyncEnabled_(true),
		isWindowless_(false),
		windowStyle_(sf::Style::Default),
		videoMode_(800, 600)
{
	setTickRate(100);
}

Game::~Game()
{
	if (mainWindow_)
	{
		delete mainWindow_;
	}
}

void Game::start(int argc, char **argv)
{
	if (started_)
		return;

	initialize(argc, argv);

	started_ = true;

	if (!isWindowless_)
	{
		mainWindow_ = new RenderWindow(videoMode_, name_, windowStyle_);
		mainWindow_->setVerticalSyncEnabled(isVerticalSyncEnabled_);
		mainWindow_->setMouseCursorVisible(false);
	}

	Clock clock;

	const Time oneSecond = sf::milliseconds(1000);
	const Time maxFrameTime = sf::milliseconds(250);

	Time t;
	Time currentTime = clock.getElapsedTime();
	Time accumulator;
	Time fpsTime;

	int frameCount = 0;

	// these variables will prevent multiple resize event when resizing window
	bool didLastFrameResize = false;
	Vector2u wndCurrentSize;
	Vector2u wndResizeData;
	wndCurrentSize.x = videoMode_.width;
	wndCurrentSize.y = videoMode_.height;

	update(dt_.asSeconds());

	while (!quit_)
	{
		Time newTime = clock.getElapsedTime();
		Time frameTime = newTime - currentTime;

		fpsTime += frameTime;

		if (fpsTime >= oneSecond)
		{
			fpsTime -= oneSecond;
			fps_ = frameCount;
			frameCount = 0;
		}

		if (frameTime > maxFrameTime)
		{
			frameTime = maxFrameTime;
		}

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= dt_)
		{
			update(dt_.asSeconds());

			t += dt_;
			accumulator -= dt_;
		}

		if (!isWindowless_)
		{
			// Draw

			if (!didLastFrameResize)
			{
				interpolateValue_ = (accumulator / dt_.asMicroseconds()).asSeconds();

				draw(*mainWindow_);

				mainWindow_->display();

				frameCount++;
			}

			// Process window events.

			sf::Event evt;

			bool resized = false;

			while (mainWindow_->pollEvent(evt))
			{
				if (evt.type == sf::Event::Closed)
				{
					quit();
				}
				else if (evt.type == sf::Event::Resized)
				{
					didLastFrameResize = resized = true;
					wndResizeData.x = evt.size.width;
					wndResizeData.y = evt.size.height;
				}
				else if (evt.type == sf::Event::KeyPressed)
				{
					keyPressed(evt.key);
				}
				else if (evt.type == sf::Event::KeyReleased)
				{
					keyReleased(evt.key);
				}
			}

			if (!resized && didLastFrameResize)
			{
				didLastFrameResize = false;

				if (wndCurrentSize != wndResizeData)
				{
					wndCurrentSize = wndResizeData;
					windowResized(wndCurrentSize);
				}
			}
		}
		else
		{
			sf::sleep(sf::milliseconds(1));
		}
	}

	if (mainWindow_ != 0)
	{
		mainWindow_->close();
	}
}

void Game::quit()
{
	quit_ = true;
}

int Game::getFps()
{
	return fps_;
}

void Game::setTickRate(int tickrate)
{
	assert(!started_);

	dt_ = sf::seconds(1.0f / static_cast<float>(tickrate));
}

void Game::setName(const char *name)
{
	name_ = name;

	if (mainWindow_ != 0)
	{
		mainWindow_->setTitle(name);
	}
}

void Game::setWindowless(bool isWindowless)
{
	assert(!started_);

	isWindowless_ = isWindowless;
}

void Game::setVideoMode(sf::VideoMode videoMode, Uint32 style)
{
	assert(!isWindowless_);

	if (mainWindow_ != 0 && (videoMode != videoMode_ || style != windowStyle_))
	{
		mainWindow_->create(videoMode, name_, style);
		mainWindow_->setMouseCursorVisible(false);

		if (isVerticalSyncEnabled_)
		{
			mainWindow_->setVerticalSyncEnabled(isVerticalSyncEnabled_);
		}
	}

	videoMode_ = videoMode;
	windowStyle_ = style;
}

RenderWindow &Game::getRenderWindow()
{
	assert(mainWindow_ != 0);

	return *mainWindow_;
}

void Game::interpolatePosition(Entity &entity)
{
	entity.drawPosition_ = entity.position_;

	if (entity.position_ != entity.previousPosition_)
	{
		entity.drawPosition_ = entity.position_ * interpolateValue_ + entity.previousPosition_ * (1.0f - interpolateValue_);
	}

	entity.drawPosition_.x = std::floor(entity.drawPosition_.x);
	entity.drawPosition_.y = std::floor(entity.drawPosition_.y);
}

void Game::setVerticalSync(bool enabled)
{
	isVerticalSyncEnabled_ = enabled;

	if (mainWindow_ != 0)
	{
		mainWindow_->setVerticalSyncEnabled(enabled);
	}
}

bool Game::isVerticalSyncEnabled()
{
	return isVerticalSyncEnabled_;
}
