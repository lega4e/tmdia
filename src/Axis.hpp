#ifndef LIS_SFML_AXIS_HPP_83777190
#define LIS_SFML_AXIS_HPP_83777190

#include <cmath>
#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

#include <lis/point.hpp>
#include <lis/SFML/Line.hpp>





namespace lis
{





/*
 * Класс для рисования оси с пометками.
 * Использовать следующим образом:
 *
 * 1. Создать экземпляр
 * 2. Указать начало и конец оси (отрезок)
 * 3. Если нужно рисовать текстовые пометки,
 *    обязательно нужно установить шрифт
 * 4. Настроить произвольно
 * 5. Установить места, где нужны пометки
 * 6. Отрисовать
 *
 * Пример.
 *
 * Axis axis;
 * axis.pos( {0, 0}, {1000, 0} );
 * axis.font(font);
 * axis.axisThick(3);
 * axis.axisColor(Color::Red);
 * axis.tips( {
 * 	 { 250, "250" },
 * 	 { 500, "500" },
 * 	 { 750, "750" }
 * } );
 * window.draw(axis);
 */
class Axis: public sf::Drawable, public sf::Transformable
{
public:
	typedef std::vector< std::pair<float, std::string> > tips_type;


	enum TipStyle
	{
		middle, left, right
	};



	Axis(PointF beg = { 0.0f, 0.0f }, PointF end = { 0.0f, 0.0f });

	virtual void draw(
		sf::RenderTarget &target,
		sf::RenderStates states = sf::RenderStates::Default
	) const override;

	Axis &pos(PointF newbeg, PointF newend);
	std::pair<PointF, PointF> pos() const;


	/*
	 * Axis
	 */
	Axis &drawAxis(bool is);
	bool drawAxis() const;

	Axis &axisThick(float thick);
	float axisThick() const;

	Axis &axisColor(sf::Color const &color);
	sf::Color const &axisColor() const;


	/*
	 * Tip
	 */
	Axis &drawTip(bool is);
	bool drawTip() const;

	Axis &tipStyle(TipStyle style);
	TipStyle tipStyle() const;

	Axis &tips(tips_type const &atips);
	tips_type const &tips() const;

	Axis &tipThick(float thick);
	float tipThick() const;

	Axis &tipLen(float len);
	float tipLen() const;

	Axis &tipColor(sf::Color const &color);
	sf::Color const &tipColor() const;


	/*
	 * Sign
	 */
	Axis &drawSign(bool is);
	bool drawSign() const;

	Axis &font(sf::Font const &font);
	sf::Font const &font() const;

	sf::Text &text();
	sf::Text const &text() const;



private:
	mutable lis::Line<> _line;
	mutable sf::Text _text;

	PointF    _beg, _end;
	bool      _drawaxis  = true;
	float     _axisthick = 2.0f;
	sf::Color _axiscolor = sf::Color::Black;

	bool      _drawtip  = true;
	TipStyle  _tipstyle = middle;
	float     _tipthick = 2.0f;
	float     _tiplen   = 30.0f;
	sf::Color _tipcolor = sf::Color::Black;
	tips_type _tips;

	bool _drawsign;

};





}





#endif // LIS_SFML_AXIS_HPP_83777190
