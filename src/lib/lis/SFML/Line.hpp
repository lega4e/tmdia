#ifndef LIS_LINE_HPP
#define LIS_LINE_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

#include <lis/point.hpp>





namespace lis
{
	
	
	
	
	
// types
typedef PointD Vector;





// objects
constexpr double const ALPHA = 0.0000000001f;





// functions
inline double sin(Vector const &f, Vector const &s)
{
	double const flen = f.len(), slen = s.len();
	if( std::fabs(flen) < ALPHA || std::fabs(slen) < ALPHA )
		return 0.0;
	return (f.x*s.y - s.x*f.y) / ( flen * slen );
}

inline double cos( Vector const &a, Vector const &b )
{
	double const alen = a.len();
	double const blen = b.len();
	if( fabs(alen) < ALPHA || fabs(blen) < ALPHA )
		return 0.0;
	return ( a.x * b.x + a.y * b.y ) / ( alen * blen );
}

inline double angle( Vector const &a, Vector const &b )
{
	return acos(cos(a, b));
}

inline double wangle( Vector const &a, Vector const &b = { 1.0, 0.0 } )
{
	double const asin = sin(a, b);
	double const acos = cos(a, b);
	return asin < 0 ? 2*M_PI - std::acos(acos) : std::acos(acos);
}

inline double rtog(double r)
{
	return r * 180.0 / M_PI;
}

inline double gtor(double g)
{
	return g * M_PI / 180.0;
}





template<typename T = void>
class Line: public sf::Drawable
{
public:
	Line(
		PointD begin = {0.0, 0.0},
		PointD end   = {0.0, 0.0},
		sf::Color const &acolor = sf::Color::Black,
		double thickness = 1.0f
	)
	{
		pos(begin, end);
		color(acolor);
		thick(thickness);
		return;
	}

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states = sf::RenderStates::Default
	) const override
	{
		circle_.setPosition(beg_.x, beg_.y);
		target.draw(circle_, states);
		target.draw(rect_, states);
		circle_.setPosition(end_.x, end_.y);
		target.draw(circle_, states);
		return;
	}
	


	Line &thick(float thickness)
	{
		rect_.setSize({rect_.getSize().x, thickness});
		rect_.setOrigin({0.0f, thickness/2.0f});
		circle_.setRadius(thickness / 2.0);
		circle_.setOrigin( makep<double>(1, 1) * circle_.getRadius() );
		return *this;
	}

	float thick() const
	{
		return rect_.getSize().y;
	}



	Line &pos(
		PointD begin,
		PointD end
	) 
	{
		beg_ = begin;
		end_ = end;

		rect_.setSize({
			(float)beg_.dis(end),
			rect_.getSize().y
		});

		// rect_.setOrigin({0.0f, rect_.getSize().y/2});

		rect_.setPosition(begin);
		rect_.setRotation( - rtog( wangle(end - begin) ) );

		return *this;
	}

	std::pair<PointD, PointD> pos() const
	{
		return { beg_, end_ };
	}

	Line &color(sf::Color const &newcolor)
	{
		rect_.setFillColor(newcolor);
		circle_.setFillColor(newcolor);
		return *this;
	}
	sf::Color const &color() const
	{
		return rect_.getFillColor();
	}



private:
	sf::RectangleShape rect_;
	mutable sf::CircleShape circle_;
	PointD beg_, end_;



};





}





#endif
