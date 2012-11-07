#pragma once

class LinearGradient : public sf::Drawable
{
	public:

		enum Direction { kHorizontal, kVertical };

		LinearGradient();
		LinearGradient(const sf::FloatRect &rect, const sf::Color &startColor, const sf::Color &endColor, Direction direction);

		void setRect(const sf::FloatRect &rect);
		void setStartColor(const sf::Color &startColor);
		void setEndColor(const sf::Color &endColor);
		void setDirection(Direction direction);

		const sf::FloatRect &getRect() const;
		const sf::Color &getStartColor() const;
		const sf::Color &getEndColor() const;
		Direction getDirection() const;

	private:

		virtual void draw(sf::RenderTarget &renderTarget, sf::RenderStates states) const;

		sf::FloatRect rect_;
		bool isHorizontal_;
		sf::Color startColor_;
		sf::Color endColor_;

		sf::Vertex vertices_[4];
};
