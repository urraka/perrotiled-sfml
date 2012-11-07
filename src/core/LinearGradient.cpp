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
	:	isHorizontal_(direction == LinearGradient::kHorizontal),
		rect_(rect),
		startColor_(startColor),
		endColor_(endColor)
{
}

void LinearGradient::setRect(const sf::FloatRect &rect)
{
	rect_ = rect;
}

void LinearGradient::setStartColor(const sf::Color &startColor)
{
	startColor_ = startColor;
}

void LinearGradient::setEndColor(const sf::Color &endColor)
{
	endColor_ = endColor;
}

void LinearGradient::setDirection(Direction direction)
{
	isHorizontal_ = (direction == LinearGradient::kHorizontal);
}

const sf::FloatRect &LinearGradient::getRect() const
{
	return rect_;
}

const sf::Color &LinearGradient::getStartColor() const
{
	return startColor_;
}

const sf::Color &LinearGradient::getEndColor() const
{
	return endColor_;
}

LinearGradient::Direction LinearGradient::getDirection() const
{
	return (isHorizontal_ ? LinearGradient::kHorizontal : LinearGradient::kVertical);
}

void LinearGradient::draw(sf::RenderTarget &renderTarget, sf::RenderStates states) const
{
	GLboolean glTextureEnabled = glIsEnabled(GL_TEXTURE_2D);

	if (glTextureEnabled == GL_TRUE) glDisable(GL_TEXTURE_2D);

	glLoadIdentity();

	glBegin(GL_QUADS);

	if (isHorizontal_)
	{
		glColor4f(startColor_.r / 255.0f, startColor_.g / 255.0f, startColor_.b / 255.0f, startColor_.a / 255.0f);

		glVertex2f(rect_.left, rect_.top);
		glVertex2f(rect_.left, rect_.top + rect_.height);

		glColor4f(endColor_.r / 255.0f, endColor_.g / 255.0f, endColor_.b / 255.0f, endColor_.a / 255.0f);

		glVertex2f(rect_.left + rect_.width, rect_.top + rect_.height);
		glVertex2f(rect_.left + rect_.width, rect_.top);
	}
	else
	{
		glColor4f(startColor_.r / 255.0f, startColor_.g / 255.0f, startColor_.b / 255.0f, startColor_.a / 255.0f);

		glVertex2f(rect_.left, rect_.top);
		glVertex2f(rect_.left + rect_.width, rect_.top);

		glColor4f(endColor_.r / 255.0f, endColor_.g / 255.0f, endColor_.b / 255.0f, endColor_.a / 255.0f);

		glVertex2f(rect_.left + rect_.width, rect_.top + rect_.height);
		glVertex2f(rect_.left, rect_.top + rect_.height);
	}

	glEnd();

	if (glTextureEnabled == GL_TRUE) glEnable(GL_TEXTURE_2D);
}
