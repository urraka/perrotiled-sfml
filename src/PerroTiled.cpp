#include "rx.h"
#include "core/SFMLGame.h"
#include "Camera.h"
#include "Map.h"
#include "Player.h"
#include "PerroTiled.h"

void PerroTiled::initialize(int argc, char **argv)
{
	rx::initialize();

	const Uint32 viewWidth = 800;
	const Uint32 viewHeight = 600;

	setName("PerroTiled");
	setVideoMode(sf::VideoMode(viewWidth, viewHeight));

	Resources::loadTextures("data/");

	map.load("map.png");
	camera.setBounds(map.getSize());
	camera.updateViewSize(Vector2u(viewWidth, viewHeight));
	camera.setObjective(&player);

	background.setRect(FloatRect(0.0f, 0.0f, static_cast<float>(viewWidth), static_cast<float>(viewHeight)));

	worldTexture = new sf::RenderTexture();
	worldTexture->create(viewWidth, viewHeight);
	worldSprite.setTexture(worldTexture->getTexture());
}

void PerroTiled::update(float dt)
{
	player.update(dt);
	camera.update(dt);
}

void PerroTiled::draw(RenderTarget &renderTarget)
{
	interpolatePosition(player);
	interpolatePosition(camera);

	worldTexture->setView(camera.getView());
	worldTexture->clear(Color::Transparent);
	worldTexture->draw(map);
	worldTexture->draw(player);
	worldTexture->display();

	renderTarget.clear();
	renderTarget.draw(background);
	renderTarget.draw(worldSprite);
}

void PerroTiled::onWindowResize(const Vector2u &size)
{
	FloatRect rc(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));

	getRenderWindow().setView(sf::View(rc));

	camera.updateViewSize(size);
	background.setRect(rc);

	delete worldTexture;
	worldTexture = new sf::RenderTexture();
	bool r = worldTexture->create(size.x, size.y);
	worldSprite = Sprite(worldTexture->getTexture());
}

Vector2f PerroTiled::getViewSize()
{
	Vector2u sz = getRenderWindow().getSize();

	return Vector2f(static_cast<float>(sz.x), static_cast<float>(sz.y));
}

PerroTiled::~PerroTiled()
{
	delete worldTexture;
}
