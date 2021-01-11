#ifndef LIS_SFML_HELP_HPP_72189
#define LIS_SFML_HELP_HPP_72189

#include <string>

#include <lis/point.hpp>

#include <SFML/Graphics.hpp>





namespace lis
{





sf::Color tocolor(std::string const &s);

inline void setCenteredPosition(sf::Text &text, PointF const &p)
{
	auto bounds = text.getGlobalBounds();
	text.setOrigin(
		0.50 * bounds.width,
		0.65 * bounds.height
	);
	text.setPosition( { p.x, p.y } );
	return;
}

inline void setCenteredPosition(sf::Text &text, float x, float y)
{
	return setCenteredPosition(text, { x, y });
}





}





#endif // LIS_SFML_HELP_HPP_72189
