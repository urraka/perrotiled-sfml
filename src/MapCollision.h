#pragma once

class MapCollision
{
	public:

		MapCollision()
			:	map_(0),
				collidesTop_(false),
				collidesBottom_(false),
				collidesLeft_(false),
				collidesRight_(false) {}

		enum CollisionType { kTop, kBottom, kLeft, kRight, kAny };

		void update();

		bool collides(CollisionType type = kAny);
		const Vector2f &getCorrectedPosition();

		void setMap(Map *map);
		void setObjectOrigin(const Vector2f &objectOrigin);
		void setObjectSize(const Vector2f &objectSize);
		void setPreviousPosition(const Vector2f &previousPosition);
		void setCurrentPosition(const Vector2f &currentPosition);

	private:

		FloatRect getObjectRectAtPosition(const Vector2f &position);
		FloatRect getBoundingBox(const FloatRect &rc1, const FloatRect &rc2);

		Map *map_;

		Vector2f objectOrigin_;
		Vector2f objectSize_;
		Vector2f previousPosition_;
		Vector2f currentPosition_;
		Vector2f correctedPosition_;

		bool collidesTop_;
		bool collidesBottom_;
		bool collidesLeft_;
		bool collidesRight_;
};
