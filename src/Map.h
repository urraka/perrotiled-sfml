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

		Vector2f getRealSize();
		const Vector2f &getTileSize();

	private:

		void draw(RenderTarget &renderTarget, sf::RenderStates states) const;

		int chooseShadow(Uint32 x, Uint32 y, bool &flipX, bool &flipY);

		enum Shadows
		{
			kShadowNone = 0,
			kShadowL,
			kShadowT,
			kShadowTL,
			kShadowLR,
			kShadowTB,
			kShadowTLR,
			kShadowTLB,
			kShadowTLBR,
			kShadowTl,
			kShadowTlR,
			kShadowTlB,
			kShadowTlRB,
			kShadowTlBr,
			kShadowTlTr,
			kShadowTlBl,
			kShadowTlTrB,
			kShadowTlBlR,
			kShadowTlBlTr,
			kShadowTlBlTrBr
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
		Vector2f tileSize_;
		Uint32 chunkSize_;
		std::vector<Tile> tiles_;
		std::vector<Layer> layers_;
};
