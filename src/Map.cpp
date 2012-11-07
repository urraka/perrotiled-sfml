#include "rx.h"
#include "core/SFMLGame.h"
#include "Camera.h"
#include "Map.h"

bool Map::load(const char *name)
{
	sf::Image image;

	if (!image.loadFromFile(String("data/maps/") + name))
	{
		return false;
	}

	name_ = name;
	size_ = image.getSize();
	tileSize_.x = 32.0f;
	tileSize_.y = 32.0f;

	tiles_.resize(size_.x * size_.y);

	int nVertices = 0;

	for (Uint32 x = 0; x < size_.x; x++)
	{
		for (Uint32 y = 0; y < size_.y; y++)
		{
			Uint32 i = x + y * size_.x;

			tiles_[i].solid = (image.getPixel(x, y) == Color::Black);

			if (tiles_[i].solid)
			{
				nVertices += 4;
			}
		}
	}

	// all tiles have the same texture hardcoded, this should change in the future

	const rx::Texture &tx = Resources::getTexture(rx::kGroundTexture);
	int n = static_cast<int>(tx.data->getSize().x / tx.size.x);

	float tmp;
	int iVertex = 0;
	Vector2f tx0, tx1;

	vertices_.resize(nVertices);

	for (Uint32 x = 0; x < size_.x; x++)
	{
		for (Uint32 y = 0; y < size_.y; y++)
		{
			Uint32 i = x + y * size_.x;
			
			if (tiles_[i].solid)
			{
				imageIndex_ = chooseShadow(x, y, flipX_, flipY_);

				tx0.x = (imageIndex_ % n) * tileSize_.x;
				tx0.y = static_cast<int>(imageIndex_ / n) * tileSize_.y;
				tx1.x = tx0.x + tileSize_.x;
				tx1.y = tx0.y + tileSize_.y;

				if (flipX_)
				{
					tmp = tx0.x;
					tx0.x = tx1.x;
					tx1.x = tmp;
				}

				if (flipY_)
				{
					tmp = tx0.y;
					tx0.y = tx1.y;
					tx1.y = tmp;
				}

				vertices_[iVertex + 0].position.x = static_cast<float>(x) * tileSize_.x;
				vertices_[iVertex + 0].position.y = static_cast<float>(y) * tileSize_.y;
				vertices_[iVertex + 0].texCoords = tx0;

				vertices_[iVertex + 1].position.x = vertices_[iVertex + 0].position.x;
				vertices_[iVertex + 1].position.y = vertices_[iVertex + 0].position.y + tileSize_.y;
				vertices_[iVertex + 1].texCoords.x = tx0.x;
				vertices_[iVertex + 1].texCoords.y = tx1.y;

				vertices_[iVertex + 2].position.x = vertices_[iVertex + 0].position.x + tileSize_.x;
				vertices_[iVertex + 2].position.y = vertices_[iVertex + 0].position.y + tileSize_.y;
				vertices_[iVertex + 2].texCoords = tx1;

				vertices_[iVertex + 3].position.x = vertices_[iVertex + 0].position.x + tileSize_.x;
				vertices_[iVertex + 3].position.y = vertices_[iVertex + 0].position.y;
				vertices_[iVertex + 3].texCoords.x = tx1.x;
				vertices_[iVertex + 3].texCoords.y = tx0.y;

				iVertex += 4;
			}
		}
	}

	return true;
}

void Map::draw(RenderTarget &renderTarget, sf::RenderStates states) const
{
	const rx::Texture &tx = Resources::getTexture(rx::kGroundTexture);

	renderTarget.draw(vertices_.data(), vertices_.size(), sf::Quads, tx.data);
}

bool Map::checkCollision(const FloatRect &rect)
{
	return false;
}

bool Map::checkCollision(Entity *entity)
{
	assert(entity != 0);

	return checkCollision(entity->getBounds());
}

bool Map::isTileSolid(Uint32 x, Uint32 y)
{
	assert(x >= 0 && y >= 0 && x < size_.x && y < size_.y);

	return tiles_[x + y * size_.x].solid;
}

Vector2f Map::getSize()
{
	return Vector2f(static_cast<float>(size_.x) * tileSize_.x, static_cast<float>(size_.y) * tileSize_.y);
}

int Map::chooseShadow(Uint32 x, Uint32 y, bool &flipX, bool &flipY)
{
	enum { kTop = 0, kRight, kBottom, kLeft, kTopLeft, kTopRight, kBottomRight, kBottomLeft };

	const Uint32 w = size_.x;
	const Uint32 h = size_.y;

	const bool isDefaultSolid = true;

	bool tile[8] = {};
	bool shadow[8] = {};
	int nInnerCorners = 0;
	int nSideShadows = 0;

	tile[kTop]    = (y > 0       ? isTileSolid(x, y - 1) : isDefaultSolid);
	tile[kRight]  = (x < (w - 1) ? isTileSolid(x + 1, y) : isDefaultSolid);
	tile[kBottom] = (y < (h - 1) ? isTileSolid(x, y + 1) : isDefaultSolid);
	tile[kLeft]   = (x > 0       ? isTileSolid(x - 1, y) : isDefaultSolid);

	tile[kTopLeft]     = (x > 0 && y > 0             ? isTileSolid(x - 1, y - 1) : isDefaultSolid);
	tile[kTopRight]    = (x < (w - 1) && y > 0       ? isTileSolid(x + 1, y - 1) : isDefaultSolid);
	tile[kBottomRight] = (x < (w - 1) && y < (h - 1) ? isTileSolid(x + 1, y + 1) : isDefaultSolid);
	tile[kBottomLeft]  = (x > 0 && y < (h - 1)       ? isTileSolid(x - 1, y + 1) : isDefaultSolid);

	shadow[kTop]    = !tile[kTop];
	shadow[kRight]  = !tile[kRight];
	shadow[kBottom] = !tile[kBottom];
	shadow[kLeft]   = !tile[kLeft];

	shadow[kTopLeft]     = tile[kTop]    && tile[kLeft]  && !tile[kTopLeft];
	shadow[kTopRight]    = tile[kTop]    && tile[kRight] && !tile[kTopRight];
	shadow[kBottomLeft]  = tile[kBottom] && tile[kLeft]  && !tile[kBottomLeft];
	shadow[kBottomRight] = tile[kBottom] && tile[kRight] && !tile[kBottomRight];

	for (int i = kTop; i <= kLeft; i++)
	{
		if (shadow[i]) nSideShadows++;
	}

	for (int i = kTopLeft; i <= kBottomLeft; i++)
	{
		if (shadow[i]) nInnerCorners++;
	}

	flipX = false;
	flipY = false;

	if (nInnerCorners == 0)
	{
		if (nSideShadows == 0)
		{
			return kShadowNone;
		}
		else if (nSideShadows == 1)
		{
			flipX = shadow[kRight];
			flipY = shadow[kBottom];

			if (shadow[kLeft] || shadow[kRight])
			{
				return kShadowL;
			}

			return kShadowT;
		}
		else if (nSideShadows == 2)
		{
			bool isAdjacent = (shadow[kTop] && (shadow[kLeft] || shadow[kRight]) || shadow[kBottom] && (shadow[kLeft] || shadow[kRight]));

			if (isAdjacent)
			{
				if (shadow[kRight]) flipX = true;
				if (shadow[kBottom]) flipY = true;

				return kShadowTL;
			}
			else
			{
				if (shadow[kLeft]) return kShadowLR;

				return kShadowTB;
			}
		}
		else if (nSideShadows == 3)
		{
			if (!shadow[kLeft]) flipX = true;
			if (!shadow[kTop]) flipY = true;
			
			if (!shadow[kTop] || !shadow[kBottom]) return kShadowTLR;

			return kShadowTLB;
		}
		else
		{
			return kShadowTLBR;
		}
	}
	else if (nInnerCorners == 1)
	{
		flipX = shadow[kTopRight] || shadow[kBottomRight];
		flipY = shadow[kBottomLeft] || shadow[kBottomRight];

		if (nSideShadows == 0) return kShadowTl;
		if (nSideShadows == 2) return kShadowTlRB;
		if (nSideShadows == 1 && (shadow[kLeft] || shadow[kRight])) return kShadowTlR;

		return kShadowTlB;
	}
	else if (nInnerCorners == 2)
	{
		if (shadow[kTopLeft] && shadow[kBottomRight] || shadow[kBottomLeft] && shadow[kTopRight])
		{
			flipX = shadow[kTopRight];

			return kShadowTlBr;
		}

		flipX = flipY = shadow[kBottomRight];
			
		if (shadow[kTopLeft] && shadow[kTopRight] || shadow[kBottomLeft] && shadow[kBottomRight]) return kShadowTlTr + (nSideShadows == 1 ? 2 : 0);

		return kShadowTlBl + (nSideShadows == 1 ? 2 : 0);
	}
	else if (nInnerCorners == 3)
	{
		flipX = !shadow[kBottomLeft] || !shadow[kTopLeft];
		flipY = !shadow[kTopLeft] || !shadow[kTopRight];

		return kShadowTlBlTr;
	}
	else
	{
		return kShadowTlBlTrBr;
	}
}
