#include "rx.h"
#include "core/SFMLGame.h"
#include "Camera.h"
#include "Map.h"
#include "MapCollision.h"
#include "Player.h"
#include "PerroTiled.h"

void PerroTiled::initialize(int argc, char **argv)
{
	rx::initialize();

	const Vector2u viewSize(800, 600);

	setName("PerroTiled");
	setVideoMode(sf::VideoMode(viewSize.x, viewSize.y));

	Resources::loadTextures("data/");

	map.load("map.png");

	background.setRect(FloatRect(Vector2f(0.0f, 0.0f), Vector2f(viewSize)));

	worldTexture = new sf::RenderTexture();
	worldTexture->create(viewSize.x, viewSize.y);
	worldSprite.setTexture(worldTexture->getTexture());

	Player::Controls controls;

	controls.left = Keyboard::Left;
	controls.right = Keyboard::Right;
	controls.jump = Keyboard::Up;

	players[0].init(rx::kRuby, controls, &map);

	controls.left = Keyboard::A;
	controls.right = Keyboard::D;
	controls.jump = Keyboard::W;

	players[1].init(rx::kPerro, controls, &map);

	camera.setBounds(map.getRealSize());
	camera.updateViewSize(viewSize);
	camera.setObjective(&players[0]);
}

void PerroTiled::update(float dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Escape)) quit();

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		if (!keySpace)
		{
			if (camera.getObjective() == &players[0])
			{
				camera.setObjective(&players[1]);
			}
			else
			{
				camera.setObjective(&players[0]);
			}
		}

		keySpace = true;
	}
	else
	{
		keySpace = false;
	}

	players[0].update(dt);
	players[1].update(dt);
	camera.update(dt);
}

void PerroTiled::draw(RenderTarget &renderTarget)
{
	interpolatePosition(players[0]);
	interpolatePosition(players[1]);
	interpolatePosition(camera);

	worldTexture->setView(camera.getView());
	worldTexture->clear(Color::Transparent);
	worldTexture->draw(map);
	worldTexture->draw(players[0]);
	worldTexture->draw(players[1]);
	worldTexture->display();

	renderTarget.clear();
	renderTarget.draw(background);
	renderTarget.draw(worldSprite);
}

void PerroTiled::onWindowResize(const Vector2u &size)
{
	FloatRect rc(Vector2f(0, 0), Vector2f(size));

	getRenderWindow().setView(sf::View(rc));

	camera.updateViewSize(size);
	background.setRect(rc);

	delete worldTexture;
	worldTexture = new sf::RenderTexture();
	bool r = worldTexture->create(size.x, size.y);
	worldSprite = Sprite(worldTexture->getTexture());
}

PerroTiled::~PerroTiled()
{
	delete worldTexture;
}
