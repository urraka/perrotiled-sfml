#include "rx.h"
#include "core/SFMLGame.h"
#include "Camera.h"

void Camera::update(float dt)
{
	Entity::update(dt);

	Vector2f objectivePosition(position_);

	if (objective_)
	{
		objectivePosition = objective_->getPosition();
	}

	if (view_.getSize().x > bounds_.x)
	{
		velocity_.x = 0.0f;
		position_.x = objectivePosition.x = bounds_.x / 2.0f;
	}
	else
	{
		float min = view_.getSize().x / 2.0f;
		float max = bounds_.x - view_.getSize().x / 2.0f;

		objectivePosition.x = std::min(std::max(min, objectivePosition.x), max);
		position_.x = std::min(std::max(min, position_.x), max);

		float maxDistance;
		float cameraDistance = objectivePosition.x - position_.x;

		maxDistance = view_.getSize().x / 2.0f - 50.0f;

		if (!changingObjectiveX_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.x = 0.0f;
			position_.x = objectivePosition.x - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveX_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveX_ = false;

			velocity_.x = cameraDistance * 2.5f;
		}
	}

	if (view_.getSize().y > bounds_.y)
	{
		velocity_.y = 0.0f;
		position_.y = objectivePosition.y = bounds_.y / 2.0f;
	}
	else
	{
		float min = view_.getSize().y / 2.0f;
		float max = bounds_.y - view_.getSize().y / 2.0f;

		objectivePosition.y = std::min(std::max(min, objectivePosition.y), max);
		position_.y = std::min(std::max(min, position_.y), max);

		float maxDistance;
		float cameraDistance = objectivePosition.y - position_.y;

		maxDistance = view_.getSize().y / 2.0f - 50.0f;

		if (!changingObjectiveY_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.y = 0.0f;
			position_.y = objectivePosition.y - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveY_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveY_ = false;

			velocity_.y = cameraDistance * 2.5f;
		}
	}

	// set a minimun velocity

	if ((velocity_.x * velocity_.x + velocity_.y * velocity_.y) < (kMinVel * kMinVel))
	{
		velocity_.x = 0.0f;
		velocity_.y = 0.0f;
	}

	// update position

	position_ += velocity_ * dt;
}

void Camera::moveToObjective()
{
	if (objective_)
	{
		position_ = objective_->getPosition();
	}
}

void Camera::setObjective(Entity *objective)
{
	objective_ = objective;
	changingObjectiveX_ = true;
	changingObjectiveY_ = true;
}

Entity *Camera::getObjective()
{
	return objective_;
}

void Camera::setBounds(const Vector2f &bounds)
{
	bounds_ = bounds;
}

void Camera::updateViewSize(const Vector2u &viewSize)
{
	view_.setSize(static_cast<float>(viewSize.x), static_cast<float>(viewSize.y));
}

const sf::View &Camera::getView()
{
	Vector2f offset = view_.getSize() / 2.0f;

	offset.x = std::floor(offset.x);
	offset.y = std::floor(offset.y);

	view_.reset(FloatRect(drawPosition_ - offset, view_.getSize()));

	return view_;
}
