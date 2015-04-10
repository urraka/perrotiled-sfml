#include "common.h"
#include "Resources.h"
#include "Animation.h"
#include "Entity.h"

void Entity::update(float dt)
{
	previousPosition_ = position_;
}

void Entity::draw(RenderTarget &renderTarget, sf::RenderStates states) const
{
	const rx::Texture &tx = Resources::getTexture(textureId_);

	assert(tx.data != 0);

	Vector2u totalSize = tx.data->getSize();

	float x0 = static_cast<float>(-tx.origin.x);
	float y0 = static_cast<float>(-tx.origin.y);
	float w = static_cast<float>(totalSize.x);
	float h = static_cast<float>(totalSize.y);

	Vector2f tx0(0.0f, 0.0f);
	Vector2f tx1(w, h);

	if (tx.size.x > 0 && tx.size.y > 0)
	{
		w = static_cast<float>(tx.size.x);
		h = static_cast<float>(tx.size.y);

		int n = static_cast<int>(totalSize.x / tx.size.x);

		tx0.x = (imageIndex_ % n) * w;
		tx0.y = static_cast<int>(imageIndex_ / n) * h;

		tx1.x = tx0.x + w;
		tx1.y = tx0.y + h;
	}

	if (flipX_)
	{
		float tmp = tx0.x;
		tx0.x = tx1.x;
		tx1.x = tmp;
	}

	if (flipY_)
	{
		float tmp = tx0.y;
		tx0.y = tx1.y;
		tx1.y = tmp;
	}

	states.texture = tx.data;
	states.transform.translate(drawPosition_);

	sf::Vertex vertices[4];

	vertices[0].position.x = x0;
	vertices[0].position.y = y0;
	vertices[0].texCoords.x = tx0.x;
	vertices[0].texCoords.y = tx0.y;

	vertices[1].position.x = x0;
	vertices[1].position.y = y0 + h;
	vertices[1].texCoords.x = tx0.x;
	vertices[1].texCoords.y = tx1.y;

	vertices[2].position.x = x0 + w;
	vertices[2].position.y = y0 + h;
	vertices[2].texCoords.x = tx1.x;
	vertices[2].texCoords.y = tx1.y;

	vertices[3].position.x = x0 + w;
	vertices[3].position.y = y0;
	vertices[3].texCoords.x = tx1.x;
	vertices[3].texCoords.y = tx0.y;

	renderTarget.draw(vertices, 4, sf::Quads, states);
}

bool Entity::checkCollision(Entity *entity)
{
	return getBounds().intersects(entity->getBounds());
}

FloatRect Entity::getBounds()
{
	if (textureId_ == -1) return FloatRect();

	const rx::Texture &tx = Resources::getTexture(textureId_);

	assert(tx.data != 0);

	FloatRect rc;

	if (tx.size.x > 0 && tx.size.y > 0)
	{
		rc.width = static_cast<float>(tx.size.x);
		rc.height = static_cast<float>(tx.size.y);
	}
	else
	{
		Vector2u sz = tx.data->getSize();

		rc.width = static_cast<float>(sz.x);
		rc.height = static_cast<float>(sz.y);
	}

	rc.left = position_.x - static_cast<float>(tx.origin.x);
	rc.top = position_.y - static_cast<float>(tx.origin.y);

	return rc;
}

void Entity::setImage(int textureId, int imageIndex)
{
	textureId_ = textureId;
	imageIndex_ = imageIndex;
}

const Vector2f &Entity::getPosition() const
{
	return position_;
}

const Vector2f &Entity::getVelocity() const
{
	return velocity_;
}
