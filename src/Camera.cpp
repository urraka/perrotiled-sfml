#include "rx.h"
#include "core/SFMLGame.h"
#include "Camera.h"

void Camera::update2(float dt)
{
	Entity::update(dt);

	const float kVelMultiplier = 2.5f;
	const float kMinVel = 20.0f;

	Vector2f objectivePosition(position_);
	Vector2f objectiveVelocity;

	if (objective_)
	{
		objectivePosition = objective_->getPosition();
		objectiveVelocity = objective_->getVelocity();
	}

	if (view_.getSize().x > bounds_.x)
	{
		velocity_.x = 0.0f;
		acceleration_.x = 0.0f;
		position_.x = objectivePosition.x = bounds_.x / 2.0f;
	}
	else
	{
		float cameraDistance = objectivePosition.x - position_.x;
		float maxDistance = view_.getSize().x / 2.0f - 50.0f;

		if (!changingObjectiveX_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.x = 0.0f;
			acceleration_.x = 0.0f;
			position_.x = objectivePosition.x - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveX_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveX_ = false;

			acceleration_.x = -pow(velocity_.x, 2) / (2 * cameraDistance);
		}
	}

	if (view_.getSize().y > bounds_.y)
	{
		velocity_.y = 0.0f;
		acceleration_.y = 0.0f;
		position_.y = objectivePosition.y = bounds_.y / 2.0f;
	}
	else
	{
		float cameraDistance = objectivePosition.y - position_.y;
		float maxDistance = view_.getSize().y / 2.0f - 50.0f;

		if (!changingObjectiveY_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.y = 0.0f;
			acceleration_.y = 0.0f;
			position_.y = objectivePosition.y - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveY_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveY_ = false;

			acceleration_.y = objectiveVelocity.y - velocity_.y;
		}
	}

	// set a minimun velocity

	/*if ((velocity_.x * velocity_.x + velocity_.y * velocity_.y) < (kMinVel * kMinVel))
	{
		velocity_.x = 0.0f;
		velocity_.y = 0.0f;
	}*/

	// update position

	velocity_ += acceleration_ * dt;
	position_ += velocity_ * dt;

	Vector2f min = view_.getSize() / 2.0f;
	Vector2f max = bounds_ - min;

	position_.x = std::min(std::max(min.x, position_.x), max.x);
	position_.y = std::min(std::max(min.y, position_.y), max.y);
}

void Camera::update(float dt)
{
	//update2(dt);
	//return;

	Entity::update(dt);

	const float kVelMultiplier = 2.5f;
	const float kMinVel = 20.0f;

	Vector2f objectivePosition(position_);
	Vector2f objectiveVelocity;

	if (objective_)
	{
		objectivePosition = objective_->getPosition();
		objectiveVelocity = objective_->getVelocity();
	}

	if (view_.getSize().x > bounds_.x)
	{
		velocity_.x = 0.0f;
		position_.x = objectivePosition.x = bounds_.x / 2.0f;
	}
	else
	{
		float cameraDistance = objectivePosition.x - position_.x;
		float maxDistance = view_.getSize().x / 2.0f - 50.0f;

		if (!changingObjectiveX_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.x = 0.0f;
			position_.x = objectivePosition.x - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveX_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveX_ = false;

			velocity_.x = cameraDistance * kVelMultiplier;
		}
	}

	if (view_.getSize().y > bounds_.y)
	{
		velocity_.y = 0.0f;
		position_.y = objectivePosition.y = bounds_.y / 2.0f;
	}
	else
	{
		float cameraDistance = objectivePosition.y - position_.y;
		float maxDistance = view_.getSize().y / 2.0f - 50.0f;

		if (!changingObjectiveY_ && abs(cameraDistance) > maxDistance)
		{
			velocity_.y = 0.0f;
			position_.y = objectivePosition.y - (cameraDistance > 0.0f ? maxDistance : -maxDistance);
		}
		else
		{
			if (changingObjectiveY_ && abs(cameraDistance) <= maxDistance)
				changingObjectiveY_ = false;

			velocity_.y = cameraDistance * kVelMultiplier;
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

	Vector2f min = view_.getSize() / 2.0f;
	Vector2f max = bounds_ - min;

	position_.x = std::min(std::max(min.x, position_.x), max.x);
	position_.y = std::min(std::max(min.y, position_.y), max.y);
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
	//view_.zoom(1.7f);
}

const sf::View &Camera::getView()
{
	Vector2f offset = view_.getSize() / 2.0f;

	offset.x = std::floor(offset.x);
	offset.y = std::floor(offset.y);

	view_.reset(FloatRect(drawPosition_ - offset, view_.getSize()));

	return view_;
}
