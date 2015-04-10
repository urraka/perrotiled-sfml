#include "rx.h"
#include "core/SFMLGame.h"
#include "Map.h"
#include "MapCollision.h"
#include "Player.h"

void Player::init(int texture, const Controls &controls, Map *map)
{
	textureId_ = texture;
	controls_ = controls;
	position_ = map->getSpawnPoint();

	FloatRect bounds = getBounds();
	Vector2f origin = -(Vector2f(bounds.left, bounds.top) - position_);
	Vector2f size = Vector2f(bounds.width, bounds.height);

	mapCollision_.setMap(map);
	mapCollision_.setObjectOrigin(origin);
	mapCollision_.setObjectSize(size);
}

void Player::respawn(Map *map)
{
	velocity_.x = 0.0f;
	velocity_.y = 0.0f;
	position_ = map->getSpawnPoint();
}

void Player::update(float dt)
{
	Entity::update(dt);

	const float kWalkAcc = 800.0f;
	const float kMaxWalkVel = 250.0f;
	const float kJumpVel = 550.0f;
	const float kGravity = 9.8f * 150.0f;

	acceleration_.x = 0.0f;
	acceleration_.y = kGravity;

	Action action = processInput();

	if (action.goRight)
	{
		flipX_ = true;
		velocity_.x = kMaxWalkVel;
	}
	else if (action.goLeft)
	{
		flipX_ = false;
		velocity_.x = -kMaxWalkVel;
	}
	else
	{
		velocity_.x = 0.0f;
	}

	if (action.jump && mapCollision_.collides(MapCollision::kBottom))
	{
		velocity_.y = -kJumpVel;
	}

	velocity_ += acceleration_ * dt;
	position_ += velocity_ * dt;

	mapCollision_.setPreviousPosition(previousPosition_);
	mapCollision_.setCurrentPosition(position_);
	mapCollision_.update();

	position_ = mapCollision_.getCorrectedPosition();

	if (mapCollision_.collides(MapCollision::kTop) || mapCollision_.collides(MapCollision::kBottom))
	{
		velocity_.y = 0.0f;
	}

	/*if (mapCollision_.collides(MapCollision::kBottom))
	{
		velocity_.y = 1.0f / dt;
	}*/

	if (mapCollision_.collides(MapCollision::kBottom))
	{
		if (velocity_.x != 0.0f && !(mapCollision_.collides(MapCollision::kLeft) || mapCollision_.collides(MapCollision::kRight)))
		{
			if (!animation_.is(rx::kWalking))
			{
				animation_.set(rx::kWalking, 1);
			}
		}
		else
		{
			animation_.set(rx::kStanding);
		}
	}
	else
	{
		animation_.set(rx::kJumping);
	}

	animation_.update(dt);

	setImage(textureId_, animation_.getCurrentImageIndex());
}

void Player::keyPressed(const sf::Event::KeyEvent &keyEvent)
{
}

Player::Action Player::processInput()
{
	Action action;

	if (Keyboard::isKeyPressed(controls_.left))
	{
		action.goLeft = true;
	}
	else if (Keyboard::isKeyPressed(controls_.right))
	{
		action.goRight = true;
	}

	if (Keyboard::isKeyPressed(controls_.jump))
	{
		action.jump = true;
	}

	return action;
}

FloatRect Player::getBounds()
{
	FloatRect bounds = Entity::getBounds();

	bounds.left += 14.0f;
	bounds.top += 3.0f;
	bounds.width -= 28.0f;
	bounds.height -= 6.0f;

	return bounds;
}
