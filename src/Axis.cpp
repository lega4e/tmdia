#include "Axis.hpp"

#include <iostream>
#include <stdexcept>


using namespace std;





namespace lis
{





Axis::Axis(PointF beg, PointF end):
	_beg(beg), _end(end)
{
	_text.setFillColor(sf::Color::Black);
}

void Axis::draw(
	sf::RenderTarget &target,
	sf::RenderStates states
) const
{
	auto t = getTransform();
	states.transform *= t;

	if(_drawaxis)
	{
		_line.pos(_beg, _end);
		_line.thick(_axisthick);
		_line.color(_axiscolor);

		target.draw(_line, states);
	}

	if(_drawtip)
	{
		_line.thick(_tipthick);
		_line.color(_tipcolor);

		PointF dir  = (_end - _beg) / _beg.dis(_end);
		PointF norm = makep( -dir.y, dir.x );
		PointF center;
		PointF beg, end;

		for(auto b = _tips.begin(), e = _tips.end(); b != e; ++b)
		{
			center = _beg + dir * b->first;
			switch(_tipstyle)
			{
			case middle:
				beg = center + norm * (_tiplen / 2.0f);
				end = center - norm * (_tiplen / 2.0f);
				break;
			case left:
				beg = center - norm * _tiplen;
				end = center;
				break;
			case right:
				beg = center;
				end = center + norm * _tiplen;
				break;
			default:
				throw std::logic_error("Axis::draw() -> unknown tip style");
			}

			_line.pos(beg, end);
			target.draw(_line, states);
		}
	}

	if(_drawsign)
	{
		PointF dir  = (_end - _beg) / _beg.dis(_end);
		// PointF norm = makep( -dir.y, dir.x );
		PointF center;

		for(auto b = _tips.begin(), e = _tips.end(); b != e; ++b)
		{
			center = _beg + dir * b->first;
			_text.setString(b->second);
			_text.setPosition( {
				center.x,
				center.y
			} );
			target.draw(_text, states);
		}
	}

	return;
}


Axis &Axis::pos(PointF newbeg, PointF newend)
{
	_beg = newbeg;
	_end = newend;
	return *this;
}

std::pair<PointF, PointF> Axis::pos() const
{
	return { _beg, _end };
}



/*
 * Axis
 */
Axis &Axis::drawAxis(bool is)
{
	_drawaxis = is;
	return *this;
}

bool Axis::drawAxis() const
{
	return _drawaxis;
}


Axis &Axis::axisThick(float thick)
{
	_axisthick = thick;
	return *this;
}

float Axis::axisThick() const
{
	return _axisthick;
}

Axis &Axis::axisColor(sf::Color const &color)
{
	_axiscolor = color;
	return *this;
}

sf::Color const &Axis::axisColor() const
{
	return _axiscolor;
}



/*
 * Tip
 */
Axis &Axis::drawTip(bool is)
{
	_drawtip = is;
	return *this;
}

bool Axis::drawTip() const
{
	return _drawtip;
}


Axis &Axis::tipStyle(TipStyle style)
{
	_tipstyle = style;
	return *this;
}

Axis::TipStyle Axis::tipStyle() const
{
	return _tipstyle;
}


Axis &Axis::tips(tips_type const &atips)
{
	_tips = atips;
	return *this;
}

Axis::tips_type const &Axis::tips() const
{
	return _tips;
}


Axis &Axis::tipThick(float thick)
{
	_tipthick = thick;
	return *this;
}

float Axis::tipThick() const
{
	return _tipthick;
}


Axis &Axis::tipLen(float len)
{
	_tiplen = len;
	return *this;
}

float Axis::tipLen() const
{
	return _tiplen;
}


Axis &Axis::tipColor(sf::Color const &color)
{
	_tipcolor = color;
	return *this;
}

sf::Color const &Axis::tipColor() const
{
	return _tipcolor;
}



/*
 * Sign
 */
Axis &Axis::drawSign(bool is)
{
	_drawsign = is;
	return *this;
}

bool Axis::drawSign() const
{
	return _drawsign;
}


Axis &Axis::font(sf::Font const &font)
{
	_text.setFont(font);
	return *this;
}

sf::Font const &Axis::font() const
{
	return *_text.getFont();
}


sf::Text &Axis::text()
{
	return _text;
}

sf::Text const &Axis::text() const
{
	return _text;
}





}





// end
