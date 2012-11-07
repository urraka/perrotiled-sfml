#pragma once

#define SFML_STATIC

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>

typedef std::string String;

// common types

using sf::Int8;
using sf::Uint8;
using sf::Int16;
using sf::Uint16;
using sf::Int32;
using sf::Uint32;
using sf::Int64;
using sf::Uint64;

// common rect types

using sf::IntRect;
using sf::FloatRect;

// common vector types

using sf::Vector2i;
using sf::Vector2u;
using sf::Vector2f;

// usefull classes

using sf::Time;
using sf::Clock;
using sf::Keyboard;
using sf::RenderWindow;
using sf::RenderTarget;
using sf::VideoMode;
using sf::Texture;
using sf::Sprite;
using sf::Color;

const double PI = std::atan(1.0) * 4;

template <typename T> inline String str(const T& t)
{
	std::ostringstream os;
	os << t;
	return os.str();
}
