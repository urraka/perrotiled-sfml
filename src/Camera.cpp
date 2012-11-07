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

	// keep camera within map bounds

	const Vector2f cameraMin(view_.getSize().x / 2.0f, view_.getSize().y / 2.0f);
	const Vector2f cameraMax(bounds_.x - view_.getSize().x / 2.0f, bounds_.y - view_.getSize().y / 2.0f);

	objectivePosition.x = std::min(std::max(cameraMin.x, objectivePosition.x), cameraMax.x);
	objectivePosition.y = std::min(std::max(cameraMin.y, objectivePosition.y), cameraMax.y);

	// this is to fix position when resizing window

	position_.x = std::min(std::max(cameraMin.x, position_.x), cameraMax.x);
	position_.y = std::min(std::max(cameraMin.y, position_.y), cameraMax.y);

	// calculate velocity

	Vector2f cameraDistance = objectivePosition - position_;

	velocity_.x = cameraDistance.x * 2.5f;
	velocity_.y = cameraDistance.y * 2.5f;

	// set a minimun velocity

	if ((velocity_.x * velocity_.x + velocity_.y * velocity_.y) < (kMinVel * kMinVel))
	{
		velocity_.x = 0.0f;
		velocity_.y = 0.0f;
	}

	// update position & zoom

	position_ += velocity_ * dt;
}

void Camera::setObjective(Entity *objective)
{
	objective_ = objective;
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
