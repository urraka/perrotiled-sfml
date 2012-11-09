#include "rx.h"
#include "core/SFMLGame.h"
#include "Map.h"
#include "MapCollision.h"

void MapCollision::update()
{
	assert(map_);

	collidesTop_ = false;
	collidesBottom_ = false;
	collidesLeft_ = false;
	collidesRight_ = false;

	const FloatRect &rcPrevious = getObjectRectAtPosition(previousPosition_);
	const FloatRect &rcCurrent = getObjectRectAtPosition(currentPosition_);

	if (!map_->checkCollision(getBoundingBox(rcPrevious, rcCurrent)))
		return;

	enum Axis { kAxisX, kAxisY };

	Axis fastAxis = kAxisX;

	float Vector2f:: *fast = &Vector2f::x;
	float Vector2f:: *slow = &Vector2f::y;

	Vector2f delta = currentPosition_ - previousPosition_;

	if (abs(delta.y) > abs(delta.x))
	{
		fastAxis = kAxisY;

		fast = &Vector2f::y;
		slow = &Vector2f::x;
	}

	delta.*slow = delta.*slow / abs(delta.*fast);
	delta.*fast = (delta.*fast > 0.0f ? 1.0f : (delta.*fast < 0.0f ? -1.0f : 0.0f));

	Vector2f pos = previousPosition_;

	while (pos != currentPosition_)
	{
		Vector2f prevPos = pos;

		// fast axis

		if (delta.*fast != 0.0f)
		{
			if (abs(currentPosition_.*fast - pos.*fast) <= abs(delta.*fast))
			{
				delta.*fast = 0.0f;
				pos.*fast = currentPosition_.*fast;
			}
			else
			{
				pos.*fast += delta.*fast;
			}

			// check collision

			if (map_->checkCollision(getObjectRectAtPosition(pos)))
			{
				Axis currentAxis = (fastAxis == kAxisX ? kAxisX : kAxisY);

				if (currentAxis == kAxisX)
				{
					if (currentPosition_.x - previousPosition_.x > 0.0f)
					{
						collidesRight_ = true;
					}
					else
					{
						collidesLeft_ = true;
					}
				}
				else
				{
					if (currentPosition_.y - previousPosition_.y > 0.0f)
					{
						collidesBottom_ = true;
					}
					else
					{
						collidesTop_ = true;
					}
				}

				delta.*fast = 0.0f;
				pos.*fast = prevPos.*fast;
				currentPosition_.*fast = prevPos.*fast;

				delta.*slow = (delta.*slow > 0.0f ? 1.0f : (delta.*slow < 0.0f ? -1.0f : 0.0f));
			}
		}

		// slow axis

		if (delta.*slow != 0.0f)
		{
			if (abs(currentPosition_.*slow - pos.*slow) <= abs(delta.*slow))
			{
				delta.*slow = 0.0f;
				pos.*slow = currentPosition_.*slow;
			}
			else
			{
				pos.*slow += delta.*slow;
			}

			// check collision

			if (map_->checkCollision(getObjectRectAtPosition(pos)))
			{
				Axis currentAxis = (fastAxis == kAxisY ? kAxisX : kAxisY);

				if (currentAxis == kAxisX)
				{
					if (currentPosition_.x - previousPosition_.x > 0.0f)
					{
						collidesRight_ = true;
					}
					else
					{
						collidesLeft_ = true;
					}
				}
				else
				{
					if (currentPosition_.y - previousPosition_.y > 0.0f)
					{
						collidesBottom_ = true;
					}
					else
					{
						collidesTop_ = true;
					}
				}

				delta.*slow = 0.0f;
				pos.*slow = prevPos.*slow;
				currentPosition_.*slow = prevPos.*slow;
			}
		}
	}

	correctedPosition_ = currentPosition_;
}

bool MapCollision::collides(CollisionType type)
{
	switch (type)
	{
		case kTop :
			
			return collidesTop_;

		case kBottom :

			return collidesBottom_;

		case kLeft :

			return collidesLeft_;

		case kRight :

			return collidesRight_;

		default :

			return collidesTop_ || collidesBottom_ || collidesLeft_ || collidesRight_;
	}
}

const Vector2f &MapCollision::getCorrectedPosition()
{
	return correctedPosition_;
}

void MapCollision::setMap(Map *map)
{
	map_ = map;
}

void MapCollision::setObjectOrigin(const Vector2f &objectOrigin)
{
	objectOrigin_ = objectOrigin;
}

void MapCollision::setObjectSize(const Vector2f &objectSize)
{
	objectSize_ = objectSize;
}

void MapCollision::setPreviousPosition(const Vector2f &previousPosition)
{
	previousPosition_ = previousPosition;
}

void MapCollision::setCurrentPosition(const Vector2f &currentPosition)
{
	currentPosition_ = currentPosition;
	correctedPosition_ = currentPosition;
}

FloatRect MapCollision::getObjectRectAtPosition(const Vector2f &position)
{
	return FloatRect(position - objectOrigin_, objectSize_);
}

FloatRect MapCollision::getBoundingBox(const FloatRect &rc1, const FloatRect &rc2)
{
	FloatRect bounds;

	bounds.left = std::min(rc1.left, rc2.left);
	bounds.top = std::min(rc1.top, rc2.top);
	bounds.width = std::max(rc1.left + rc1.width, rc2.left + rc2.width) - bounds.left;
	bounds.height = std::max(rc1.top + rc1.height, rc2.top + rc2.height) - bounds.top;

	return bounds;
}
