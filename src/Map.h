#pragma once

class Map : public Entity
{
	public:

		bool load(const char *name);

		void update(float dt) {};

		void draw(RenderTarget &renderTarget, sf::RenderStates states) const;

		bool checkCollision(const FloatRect &rect);

		bool checkCollision(Entity *entity);

		Vector2f getSize();

	private:

		bool isTileSolid(Uint32 x, Uint32 y);
		int chooseShadow(Uint32 x, Uint32 y, bool &flipX, bool &flipY);

		enum FlipMode
		{
			kFlipNone = 0,
			kFlipX = 0x01,
			kFlipY = 0x02
		};

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

		struct Tile
		{
			bool solid;

			Tile() : solid(false) {}
		};

		String name_;
		Vector2u size_;
		Vector2f tileSize_;
		Vector2f viewSize_;
		std::vector<Tile> tiles_;
		std::vector<sf::Vertex> vertices_;
};
