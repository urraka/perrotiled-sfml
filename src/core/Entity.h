#pragma once

class Game;

class Entity : public sf::Drawable
{
	public:

		Entity() : flipX_(false), flipY_(false), textureId_(-1), imageIndex_(0) {}
		virtual ~Entity() {}

		void setImage(int textureId, int imageIndex = 0);

		const Vector2f &getPosition() const;
		const Vector2f &getVelocity() const;

		virtual void update(float dt);
		virtual bool checkCollision(Entity *entity);
		virtual FloatRect getBounds();

		virtual void draw(RenderTarget &renderTarget, sf::RenderStates states) const;

	protected:

		bool flipX_;
		bool flipY_;

		int textureId_;
		int imageIndex_;

		Vector2f position_;
		Vector2f velocity_;
		Vector2f previousPosition_;
		Vector2f drawPosition_;

	friend class Game;
};
