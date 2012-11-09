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

	private:

		virtual void draw(sf::RenderTarget &renderTarget, sf::RenderStates states) const;

		void swapDirection();

		bool isHorizontal_;

		sf::Vertex vertices_[4];
};
