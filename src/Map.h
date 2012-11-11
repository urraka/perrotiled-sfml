#pragma once

class Map : public sf::Drawable
{
	public:

		Map()
			:	chunkSize_(32) {}

		bool load(const char *name);

		void update(float dt) {};

		bool isTileSolid(Uint32 x, Uint32 y);
		bool checkCollision(const FloatRect &rect);

		Vector2u getRealSize();
		Vector2u getTileSize();

		Vector2f getSpawnPoint();

	private:

		void draw(RenderTarget &renderTarget, sf::RenderStates states) const;

		int chooseShadow(Uint32 x, Uint32 y, bool &flipX, bool &flipY);

		enum Shadows
		{
			kShadowNone = 0, kShadowL, kShadowT, kShadowTL, kShadowLR, kShadowTB, kShadowTLR,
			kShadowTLB, kShadowTLBR, kShadowTl, kShadowTlR, kShadowTlB, kShadowTlRB, kShadowTlBr,
			kShadowTlTr, kShadowTlBl, kShadowTlTrB, kShadowTlBlR, kShadowTlBlTr, kShadowTlBlTrBr
		};

		struct TileType
		{
			Color color;
			String texture;
			bool solid;
		};

		struct Tile
		{
			bool solid;
			bool hasTexture;

			Tile() : solid(false), hasTexture(false) {}
		};

		struct Layer
		{
			Texture texture;
			std::vector<std::vector<sf::Vertex>> chunks;
		};

		String name_;
		String author_;

		Vector2u size_;
		Vector2u tileSize_;
		Uint32 chunkSize_;
		std::vector<Tile> tiles_;
		std::vector<Layer> layers_;
		std::vector<IntRect> spawnAreas_;
};
