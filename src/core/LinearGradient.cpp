#define SFML_STATIC

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "LinearGradient.h"

LinearGradient::LinearGradient()
	:	isHorizontal_(false)
{
}

LinearGradient::LinearGradient(const sf::FloatRect &rect, const sf::Color &startColor, const sf::Color &endColor, LinearGradient::Direction direction)
{
	isHorizontal_ = (direction == kHorizontal);

	setRect(rect);
	setStartColor(startColor);
	setEndColor(endColor);
}

void LinearGradient::swapDirection()
{
	sf::Vertex v = vertices_[1];
	vertices_[1] = vertices_[3];
	vertices_[3] = v;
}

void LinearGradient::setRect(const sf::FloatRect &rect)
{
	if (isHorizontal_) swapDirection();

	vertices_[0].position.x = rect.left;
	vertices_[0].position.y = rect.top;

	if (isHorizontal_)
	{
		vertices_[1].position.x = rect.left;
		vertices_[1].position.y = rect.top + rect.height;
	}
	else
	{
		vertices_[1].position.x = rect.left + rect.width;
		vertices_[1].position.y = rect.top;
	}

	vertices_[2].position.x = rect.left + rect.width;
	vertices_[2].position.y = rect.top + rect.height;

	if (isHorizontal_)
	{
		vertices_[3].position.x = rect.left + rect.width;
		vertices_[3].position.y = rect.top;
	}
	else
	{
		vertices_[3].position.x = rect.left;
		vertices_[3].position.y = rect.top + rect.height;
	}

	if (isHorizontal_) swapDirection();
}

void LinearGradient::setStartColor(const sf::Color &startColor)
{
	vertices_[0].color = startColor;
	vertices_[1].color = startColor;
}

void LinearGradient::setEndColor(const sf::Color &endColor)
{
	vertices_[2].color = endColor;
	vertices_[3].color = endColor;
}

void LinearGradient::setDirection(Direction direction)
{
	bool isHorizontal = (direction == LinearGradient::kHorizontal);

	if (isHorizontal_ != isHorizontal)
	{
		isHorizontal_ = isHorizontal;

		swapDirection();
	}
}

void LinearGradient::draw(sf::RenderTarget &renderTarget, sf::RenderStates states) const
{
	renderTarget.draw(vertices_, 4, sf::Quads);
}
