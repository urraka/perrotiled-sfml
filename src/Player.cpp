#include "rx.h"
#include "core/SFMLGame.h"
#include "Player.h"

void Player::update(float dt)
{
	Entity::update(dt);

	velocity_.x = 0.0f;
	velocity_.y = 0.0f;

	const float kVel = 500.0f;

	if (Keyboard::isKeyPressed(Keyboard::Left)) velocity_.x = -kVel;
	if (Keyboard::isKeyPressed(Keyboard::Right)) velocity_.x = kVel;
	if (Keyboard::isKeyPressed(Keyboard::Up)) velocity_.y = -kVel;
	if (Keyboard::isKeyPressed(Keyboard::Down)) velocity_.y = kVel;

	position_ += velocity_ * dt;
}

void Player::setDrawOffset(const Vector2f &offset)
{
	drawPosition_ -= offset;
}
