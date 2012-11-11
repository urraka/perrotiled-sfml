#include "rx.h"
#include "core/SFMLGame.h"
#include "Map.h"
#include "tinyxml2/tinyxml2.h"
#include "helpers/helpers.h"
#include <iomanip>

bool Map::load(const char *name)
{
	using tinyxml2::XMLDocument;
	using tinyxml2::XMLElement;
	using tinyxml2::XMLAttribute;
	using tinyxml2::XML_NO_ERROR;

	// load xml

	XMLDocument xmlDocument;
	String xmlFileName = String("data/maps/") + name + ".xml";

	if (xmlDocument.LoadFile(xmlFileName.c_str()) != XML_NO_ERROR)
	{
		return false;
	}

	const XMLElement *nameElement        = xmlDocument.RootElement()->FirstChildElement("name");
	const XMLElement *authorElement      = xmlDocument.RootElement()->FirstChildElement("author");
	const XMLElement *tilesElement       = xmlDocument.RootElement()->FirstChildElement("tiles");
	const XMLElement *spawnPointsElement = xmlDocument.RootElement()->FirstChildElement("spawnpoints");

	name_ = nameElement->FirstChild()->ToText()->Value();
	author_ = authorElement->FirstChild()->ToText()->Value();

	tileSize_.x = 32;
	tileSize_.y = 32;

	const XMLAttribute *tileSizeAttr = tilesElement->FindAttribute("size");

	if (tileSizeAttr)
	{
		std::vector<String> tokens;
		hlp::tokenize(tileSizeAttr->Value(), tokens, ",");

		if (tokens.size() == 2)
		{
			std::stringstream(tokens[0]) >> tileSize_.x;
			std::stringstream(tokens[1]) >> tileSize_.y;
		}
	}

	std::vector<TileType> tileTypes;
	const XMLElement *tileElement = tilesElement->FirstChildElement();

	while (tileElement)
	{
		TileType tileType;

		const XMLAttribute *colorAttr   = tileElement->FindAttribute("color");
		const XMLAttribute *textureAttr = tileElement->FindAttribute("texture");
		const XMLAttribute *solidAttr   = tileElement->FindAttribute("solid");

		Uint32 cl;
		std::stringstream hexColor(colorAttr->Value() + 1);
		hexColor >> std::hex >> cl;

		tileType.color.a = 255;
		tileType.color.r = static_cast<Uint8>((cl >> 16) & 0xFF);
		tileType.color.g = static_cast<Uint8>((cl >> 8) & 0xFF);
		tileType.color.b = static_cast<Uint8>(cl & 0xFF);

		tileType.solid = true;

		if (solidAttr && String("false") == solidAttr->Value())
		{
			tileType.solid = false;
		}

		tileType.texture = textureAttr->Value();

		tileTypes.push_back(tileType);
		tileElement = tileElement->NextSiblingElement();
	}

	// TODO: load spawn points

	const XMLElement *spawnPointElement = spawnPointsElement->FirstChildElement();

	while (spawnPointElement)
	{
		const XMLAttribute *tileAttr     = spawnPointElement->FindAttribute("tile");
		const XMLAttribute *positionAttr = spawnPointElement->FindAttribute("position");
		const XMLAttribute *rangeAttr    = spawnPointElement->FindAttribute("range");

		Vector2i tile;
		Vector2i offset;
		Vector2i minRange;
		Vector2i maxRange;

		std::vector<String> tokens;
		hlp::tokenize(tileAttr->Value(), tokens, ",");

		if (tokens.size() == 2)
		{
			std::stringstream(tokens[0]) >> tile.x;
			std::stringstream(tokens[1]) >> tile.y;
		}

		tokens.clear();
		hlp::tokenize(positionAttr->Value(), tokens, ",");

		if (tokens.size() == 2)
		{
			if (tokens[0] == "left")   offset.x = 0;               else
			if (tokens[0] == "middle") offset.x = tileSize_.x / 2; else
			if (tokens[0] == "right")  offset.x = tileSize_.x;

			if (tokens[1] == "top")    offset.y = 0;               else
			if (tokens[1] == "middle") offset.y = tileSize_.y / 2; else
			if (tokens[1] == "bottom") offset.y = tileSize_.y;
		}

		tokens.clear();

		if (rangeAttr)
		{
			hlp::tokenize(rangeAttr->Value(), tokens, ",");

			if (tokens.size() == 2)
			{
				std::vector<String> tokensx;
				std::vector<String> tokensy;

				hlp::tokenize(tokens[0], tokensx, ":");
				hlp::tokenize(tokens[1], tokensy, ":");

				if (tokensx.size() == 2 && tokensy.size() == 2)
				{
					std::stringstream(tokensx[0]) >> minRange.x;
					std::stringstream(tokensx[1]) >> maxRange.x;
					std::stringstream(tokensy[0]) >> minRange.y;
					std::stringstream(tokensy[1]) >> maxRange.y;
				}
			}
		}

		IntRect area;

		area.left   = tile.x * tileSize_.x + offset.x - minRange.x * tileSize_.x;
		area.top    = tile.y * tileSize_.y + offset.y - minRange.y * tileSize_.y;
		area.width  = maxRange.x * tileSize_.x + minRange.x * tileSize_.x;
		area.height = maxRange.y * tileSize_.y + minRange.y * tileSize_.y;

		spawnAreas_.push_back(area);
		spawnPointElement = spawnPointElement->NextSiblingElement();
	}

	// load image

	sf::Image image;
	String imgFileName = String("data/maps/") + name + ".png";

	if (!image.loadFromFile(imgFileName))
	{
		return false;
	}

	size_ = image.getSize();
	tiles_.resize(size_.x * size_.y);

	Uint32 nLayers = tileTypes.size();
	Uint32 nChunksX = size_.x / chunkSize_ + (size_.x % chunkSize_ > 0 ? 1 : 0);
	Uint32 nChunksY = size_.y / chunkSize_ + (size_.y % chunkSize_ > 0 ? 1 : 0);
	Uint32 nChunks = nChunksX * nChunksY;

	std::vector<std::vector<Uint32>> nVertices;
	nVertices.resize(nLayers);

	for (Uint32 i = 0; i < nLayers; i++)
	{
		nVertices[i].resize(nChunks);
	}

	for (Uint32 x = 0; x < size_.x; x++)
	{
		for (Uint32 y = 0; y < size_.y; y++)
		{
			Uint32 tileTypeIndex;
			Uint32 i = x + y * size_.x;
			Color color = image.getPixel(x, y);

			for (tileTypeIndex = 0; tileTypeIndex < tileTypes.size(); tileTypeIndex++)
			{
				if (tileTypes[tileTypeIndex].color == color)
					break;
			}

			if (tileTypeIndex < tileTypes.size())
			{
				tiles_[i].hasTexture = true;
				tiles_[i].solid = tileTypes[tileTypeIndex].solid;

				Uint32 iChunk = static_cast<Uint32>(x / chunkSize_) + static_cast<Uint32>(y / chunkSize_) * nChunksX;
				nVertices[tileTypeIndex][iChunk] += 4;

				// TODO: check if it should have a shadow and count shadow vertices for each chunk of the shadows layer
			}
		}
	}

	layers_.resize(nLayers + 1); // + 1 for the shadows layer added after this loop

	for (Uint32 iLayer = 0; iLayer < nLayers; iLayer++)
	{
		layers_[iLayer].texture.loadFromFile(String("data/maps/") + tileTypes[iLayer].texture);
		layers_[iLayer].texture.setRepeated(true);
		layers_[iLayer].chunks.resize(nChunks);

		Vector2u textureSize = layers_[iLayer].texture.getSize();

		for (Uint32 xChunk = 0; xChunk < nChunksX; xChunk++)
		{
			for (Uint32 yChunk = 0; yChunk < nChunksY; yChunk++)
			{
				Uint32 iChunk = xChunk + yChunk * nChunksX;
				std::vector<sf::Vertex> &vertices = layers_[iLayer].chunks[iChunk];
				vertices.resize(nVertices[iLayer][iChunk]);

				Uint32 iVertex = 0;

				Uint32 xTile0 = xChunk * chunkSize_;
				Uint32 yTile0 = yChunk * chunkSize_;
				Uint32 xTile1 = xTile0 + chunkSize_;
				Uint32 yTile1 = yTile0 + chunkSize_;

				for (Uint32 xTile = xTile0; xTile < xTile1 && xTile < size_.x; xTile++)
				{
					for (Uint32 yTile = yTile0; yTile < yTile1 && yTile < size_.y; yTile++)
					{
						Uint32 iTile = xTile + yTile * size_.x;

						if (tiles_[iTile].hasTexture && tileTypes[iLayer].color == image.getPixel(xTile, yTile))
						{
							vertices[iVertex + 0].position.x = static_cast<float>(xTile) * tileSize_.x;
							vertices[iVertex + 0].position.y = static_cast<float>(yTile) * tileSize_.y;
							vertices[iVertex + 1].position.x = vertices[iVertex + 0].position.x;
							vertices[iVertex + 1].position.y = vertices[iVertex + 0].position.y + tileSize_.y;
							vertices[iVertex + 2].position.x = vertices[iVertex + 0].position.x + tileSize_.x;
							vertices[iVertex + 2].position.y = vertices[iVertex + 0].position.y + tileSize_.y;
							vertices[iVertex + 3].position.x = vertices[iVertex + 0].position.x + tileSize_.x;
							vertices[iVertex + 3].position.y = vertices[iVertex + 0].position.y;

							if (textureSize.x > 0 && textureSize.y > 0)
							{
								Vector2f tx0;
								Vector2f tx1;

								tx0.x = static_cast<float>((xTile * tileSize_.x) % textureSize.x);
								tx0.y = static_cast<float>((yTile * tileSize_.y) % textureSize.y);
								tx1.x = tx0.x + tileSize_.x;
								tx1.y = tx0.y + tileSize_.y;

								vertices[iVertex + 0].texCoords.x = tx0.x;
								vertices[iVertex + 0].texCoords.y = tx0.y;
								vertices[iVertex + 1].texCoords.x = tx0.x;
								vertices[iVertex + 1].texCoords.y = tx1.y;
								vertices[iVertex + 2].texCoords.x = tx1.x;
								vertices[iVertex + 2].texCoords.y = tx1.y;
								vertices[iVertex + 3].texCoords.x = tx1.x;
								vertices[iVertex + 3].texCoords.y = tx0.y;
							}
							else
							{
								vertices[iVertex + 0].color = Color::Black;
								vertices[iVertex + 1].color = Color::Black;
								vertices[iVertex + 2].color = Color::Black;
								vertices[iVertex + 3].color = Color::Black;
							}

							iVertex += 4;
						}
					}
				}
			}
		}
	}

	// add shadows layer

	Uint32 iLayer = layers_.size() - 1;

	layers_[iLayer].texture.loadFromFile("data/gfx/shadows.png");
	Vector2u shadowsSize = layers_[iLayer].texture.getSize();
	Vector2f shadowSize = Vector2f(32.0f, 32.0f);

	layers_[iLayer].chunks.resize(nChunks);

	for (Uint32 xChunk = 0; xChunk < nChunksX; xChunk++)
	{
		for (Uint32 yChunk = 0; yChunk < nChunksY; yChunk++)
		{
			Uint32 iChunk = xChunk + yChunk * nChunksX;
			std::vector<sf::Vertex> &vertices = layers_[iLayer].chunks[iChunk];

			Uint32 iVertex = 0;

			Uint32 xTile0 = xChunk * chunkSize_;
			Uint32 yTile0 = yChunk * chunkSize_;
			Uint32 xTile1 = xTile0 + chunkSize_;
			Uint32 yTile1 = yTile0 + chunkSize_;

			for (Uint32 xTile = xTile0; xTile < xTile1 && xTile < size_.x; xTile++)
			{
				for (Uint32 yTile = yTile0; yTile < yTile1 && yTile < size_.y; yTile++)
				{
					if (!isTileSolid(xTile, yTile))
						continue;

					bool flipX, flipY;
					int shadow = chooseShadow(xTile, yTile, flipX, flipY);

					if (shadow != kShadowNone)
					{
						vertices.resize(vertices.size() + 4);

						vertices[iVertex + 0].position.x = static_cast<float>(xTile) * tileSize_.x;
						vertices[iVertex + 0].position.y = static_cast<float>(yTile) * tileSize_.y;
						vertices[iVertex + 1].position.x = vertices[iVertex + 0].position.x;
						vertices[iVertex + 1].position.y = vertices[iVertex + 0].position.y + tileSize_.y;
						vertices[iVertex + 2].position.x = vertices[iVertex + 0].position.x + tileSize_.x;
						vertices[iVertex + 2].position.y = vertices[iVertex + 0].position.y + tileSize_.y;
						vertices[iVertex + 3].position.x = vertices[iVertex + 0].position.x + tileSize_.x;
						vertices[iVertex + 3].position.y = vertices[iVertex + 0].position.y;

						int n = static_cast<int>(shadowsSize.x / shadowSize.x);

						Vector2f tx0, tx1;
						tx0.x = (shadow % n) * shadowSize.x;
						tx0.y = static_cast<int>(shadow / n) * shadowSize.y;
						tx1.x = tx0.x + shadowSize.x;
						tx1.y = tx0.y + shadowSize.y;

						if (flipX)
						{
							float tmp = tx0.x;
							tx0.x = tx1.x;
							tx1.x = tmp;
						}

						if (flipY)
						{
							float tmp = tx0.y;
							tx0.y = tx1.y;
							tx1.y = tmp;
						}

						vertices[iVertex + 0].texCoords.x = tx0.x;
						vertices[iVertex + 0].texCoords.y = tx0.y;
						vertices[iVertex + 1].texCoords.x = tx0.x;
						vertices[iVertex + 1].texCoords.y = tx1.y;
						vertices[iVertex + 2].texCoords.x = tx1.x;
						vertices[iVertex + 2].texCoords.y = tx1.y;
						vertices[iVertex + 3].texCoords.x = tx1.x;
						vertices[iVertex + 3].texCoords.y = tx0.y;

						iVertex += 4;
					}
				}
			}
		}
	}

	return true;
}

void Map::draw(RenderTarget &renderTarget, sf::RenderStates states) const
{
	const sf::View &view = renderTarget.getView();
	const Vector2f &viewSize = view.getSize();
	Vector2f viewPosition = view.getCenter() - viewSize / 2.0f;

	Uint32 nChunksX = size_.x / chunkSize_ + (size_.x % chunkSize_ > 0 ? 1 : 0);
	Uint32 nChunksY = size_.y / chunkSize_ + (size_.y % chunkSize_ > 0 ? 1 : 0);

	Uint32 x0 = static_cast<Uint32>(std::max(0.0f, viewPosition.x) / tileSize_.x) / chunkSize_;
	Uint32 y0 = static_cast<Uint32>(std::max(0.0f, viewPosition.y) / tileSize_.y) / chunkSize_;
	Uint32 x1 = std::min(nChunksX - 1, x0 + static_cast<Uint32>(std::ceil(std::ceil(viewSize.x / tileSize_.x) / chunkSize_)));
	Uint32 y1 = std::min(nChunksY - 1, y0 + static_cast<Uint32>(std::ceil(std::ceil(viewSize.y / tileSize_.y) / chunkSize_)));

	std::vector<Layer>::const_iterator iLayer;
	std::vector<std::vector<sf::Vertex>>::const_iterator iChunk;

	for (iLayer = layers_.begin(); iLayer != layers_.end(); ++iLayer)
	{
		for (Uint32 x = x0; x <= x1; x++)
		{
			for (Uint32 y = y0; y <= y1; y++)
			{
				Uint32 iChunk = x + y * nChunksX;

				renderTarget.draw(iLayer->chunks[iChunk].data(), iLayer->chunks[iChunk].size(), sf::Quads, &iLayer->texture);
			}
		}
	}
}

bool Map::checkCollision(const FloatRect &rect)
{
	int x0 = static_cast<int>(std::floor(rect.left / tileSize_.x));
	int y0 = static_cast<int>(std::floor(rect.top / tileSize_.y));
	int x1 = static_cast<int>(std::floor((rect.left + rect.width) / tileSize_.x));
	int y1 = static_cast<int>(std::floor((rect.top + rect.height) / tileSize_.y));

	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			if (x >= 0 && y >= 0 && x < static_cast<int>(size_.x) && y < static_cast<int>(size_.y))
			{
				if (isTileSolid(x, y)) return true;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

bool Map::isTileSolid(Uint32 x, Uint32 y)
{
	assert(x >= 0 && y >= 0 && x < size_.x && y < size_.y);

	return tiles_[x + y * size_.x].solid;
}

Vector2u Map::getRealSize()
{
	return Vector2u(size_.x * tileSize_.x, size_.y * tileSize_.y);
}

Vector2u Map::getTileSize()
{
	return tileSize_;
}

Vector2f Map::getSpawnPoint()
{
	Vector2i point;

	int i = rand() % spawnAreas_.size();

	point.x = spawnAreas_[i].left;
	point.y = spawnAreas_[i].top;

	if (spawnAreas_[i].width > 0)
		point.x += rand() % spawnAreas_[i].width;

	if (spawnAreas_[i].height > 0)
		point.y += rand() % spawnAreas_[i].height;

	return Vector2f(point);
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
