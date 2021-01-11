#ifndef LIS_SFML_GRID_HPP_867891
#define LIS_SFML_GRID_HPP_867891

#include <vector>

#include <SFML/Graphics.hpp>

#include <lis/SFML/Line.hpp>





namespace lis
{





/*
 * Класс, чтобы рисовать сетку; полизоваться
 * ей очень просто:
 *
 * 1. Создать экземпляр
 * 2. Установить общий размер поля
 * 3. Установить точки, в которых нужно рисовать
 *    вертикальные и горизонтальные прямые
 * 4. Настроить толщину цвет использую line
 * 5. Структура готова к отрисовке!
 *
 * Пример.
 * Grid grid;
 * grid.size = { 1920, 1080 };
 * grid.horlines = { 300, 600, 900 };
 * grid.verlines = { 500, 1000, 1500 };
 * grid.line.color(Color::Yellow);
 * grid.line.thick(4.0f);
 * grid.setRotation(30); // благодаря sf::Transformable
 * window.draw(grid);
 */
template<typename T = void>
class GridTemplate: public sf::Drawable, public sf::Transformable
{
public:
	virtual void draw(
		sf::RenderTarget &target,
		sf::RenderStates states = sf::RenderStates::Default
	) const override
	{
		states.transform *= getTransform();

		if(hor)
		{
			for(auto b = horlines.begin(), e = horlines.end(); b != e; ++b)
			{
				line.pos( { 0, *b }, { size.x, *b } );
				target.draw(line, states);
			}
		}

		if(ver)
		{
			for(auto b = verlines.begin(), e = verlines.end(); b != e; ++b)
			{
				line.pos( { *b, 0 }, { *b, size.y } );
				target.draw(line, states);
			}
		}

		return;
	}



	PointF size;

	std::vector<float> horlines;
	std::vector<float> verlines;

	bool hor = true;
	bool ver = true;

	mutable Line<T> line;
};

typedef GridTemplate<void> Grid;





}





#endif // LIS_SFML_GRID_HPP_867891
