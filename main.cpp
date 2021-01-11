#include <codecvt>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include <lis/iostream.hpp>
#include <lis/point.hpp>
#include <lis/nlohmann/json.hpp>
#include <lis/SFML/Line.hpp>
#include <lis/SFML/help.hpp>

#include "TimeDia.hpp"


using namespace lis;
using namespace lis::td;
using namespace sf;
using namespace std;





// global objects
typedef struct {
	wstring s;
	string g;
} interdata_type;

typedef sf::Color groupdata_type;

typedef shared_ptr<interdata_type> value_type;
typedef TimeDia<value_type> dia_type;


RenderWindow window;
VideoMode vmode = VideoMode::getDesktopMode();
char const *TITLE = "Application";
unsigned int FRAMERATE_LIMIT = 60u;
Font font;
RectangleShape rect;
Text text;
Text tiptext;

dia_type dia;
map<string, groupdata_type> groups;
int space = 5;
int margin = 50;
int step = 50;
int tiph = 20;

double rat;
int height = 30;






// init
void init_window()
{
	window.create(vmode, TITLE, Style::None);
	window.setPosition({0u, 0u});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}

void init_font()
{
	font.loadFromFile("times.ttf");

	text.setFont(font);
	text.setFillColor(Color::Black);
	text.setCharacterSize(15);
	text.setStyle(Text::Bold);

	tiptext.setFillColor(Color::Black);
	tiptext.setCharacterSize(15);
	tiptext.setStyle(Text::Bold);

	return;
}

wstring stows(string const &s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s.data());
}

template<typename Iterator>
void ordering(Iterator beg, Iterator end)
{
	sort(beg, end);
}

void init_dia()
{
	using namespace nlohmann;

	json data;
	cin >> data;

	json grs = data["groups"];
	for(auto b = grs.begin(), e = grs.end(); b != e; ++b)
		groups.insert({ (string)(*b)["name"], tocolor((string)(*b)["color"]) });

	map<string, INode<value_type>> nodes;

	json items = data["items"];
	wstring s;
	string g;
	for(auto b = items.begin(), e = items.end(); b != e; ++b)
	{
		s = stows((*b)["value"]);
		g = (*b)["group"];

		auto it = nodes.find(g);

		if(it == nodes.end())
			nodes.insert({ g, INode<value_type>(false) }),
			it = nodes.find(g);


		it->second.ins.push_back(
			Interval<value_type> {
				(*b)["beg"], (*b)["end"],
				value_type(new interdata_type{ s, g })
			}
		);
	}

	for(auto b = nodes.begin(), e = nodes.end(); b != e; ++b)
		ordering( b->second.ins.begin(), b->second.ins.end() ),
		dia.push(b->second);

	dia.calculate_bounds();
	rat = (vmode.width - 2*margin) / (double)(dia.max - dia.min);

	return;
}

void setPosition(Text &text, float x, float y)
{
	auto bounds = text.getGlobalBounds();
	text.setOrigin(
		0.5 * bounds.width,
		0.65 * bounds.height
	);
	text.setPosition( { x, y } );
	return;
}

void draw_dia()
{
	rect.setOutlineColor(Color::Black);
	rect.setOutlineThickness(1);
	float w, h;
	float x, y;

	h = height;
	for(int i = 0; i < (int)dia.field.size(); ++i)
	for(auto b = dia.field[i].begin(), e = dia.field[i].end(); b != e; ++b)
	{
		w = rat * (b->end - b->beg);
		x = margin + rat * (b->beg - dia.min);
		y = margin + i * (height + space);

		rect.setSize({ w, h });
		rect.setPosition(x, y); 
		rect.setFillColor(groups[b->value->g]);

		text.setString(b->value->s);
		setPosition(text, x + w/2, y + h/2);

		window.draw(rect);
		window.draw(text);
	}

	return;
}

void draw_scale()
{
	float x, y;
	x = margin;
	y = margin/2;

	lis::Line line;
	line.color(Color::Black);
	line.thick(3.0f);

	line.pos( { 0, y }, { (float)vmode.width, y } );
	window.draw(line);

	int beg = dia.min % step == 0 ? dia.min : (dia.min / step + 1) * step;
	for(; beg <= dia.max; beg += step)
	{
		line.pos(
			{ x + (beg - dia.min) * rat, y-tiph/2 },
			{ x + (beg - dia.min) * rat, y+tiph/2 }
		);
		window.draw(line);
	}

	return;
}





// main
int main( int argc, char *argv[] )
{
	std::setlocale(LC_ALL, "");
	init_window();
	init_font();
	init_dia();

	

	Event event;
	while(window.isOpen())
	{
		while(window.pollEvent(event))
		{

			switch(event.type)
			{
			case Event::KeyPressed:

				switch(event.key.code)
				{
				case Keyboard::C:
					window.close();
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}

		}

		window.clear(Color::White);
		draw_dia();
		draw_scale();
		window.display();
	}

	return 0;
}





// end
#include <lis/SFML/help.cpp>
