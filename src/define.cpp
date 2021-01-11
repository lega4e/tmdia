#include "declare.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <string>

#include <lis/mescal.hpp>
#include <lis/SFML/help.hpp>
#include <lis/SFML/Line.hpp>

#include <nlohmann/json.hpp>

#include "TimeDia.hpp"


using namespace lis;
using namespace std;
using namespace sf;





// types
typedef struct {
	std::wstring s;
	std::string g;
} interdata_type;

typedef sf::Color groupdata_type;

typedef std::shared_ptr<interdata_type> value_type;
typedef lis::td::TimeDia<value_type> dia_type;





// global objects
RenderWindow window;
VideoMode vmode = VideoMode::getDesktopMode();
char const *TITLE = "Application";
unsigned int FRAMERATE_LIMIT = 60u;



// objects
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





// functions & procedures
void init()
{
	init_window();
	init_text();
	init_dia();
	return;
}

void init_window()
{
	window.create(vmode, TITLE, Style::None);
	window.setPosition({0u, 0u});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}

void init_text()
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

/*
 * functions for read intervals from json file
 */
td::INode<value_type> read_leaf_node(
	nlohmann::json const &j,
	std::string const &group
)
{
	using namespace nlohmann;

	td::Interval<value_type> in;
	in.beg = j["beg"]; 
	in.end = j["end"];
	in.value = value_type(new interdata_type{ stows(j["value"]), group });

	return td::INode<value_type>(in);
}

td::INode<value_type> read_node(
	nlohmann::json const &j,
	std::string group = "global"
)
{
	using namespace nlohmann;

	if(j.find("items") == j.end())
		return read_leaf_node(j, group);

	td::INode<value_type> node(false);

	group = j.value("group", group);
	json items = j["items"];
	for(auto b = items.begin(), e = items.end(); b != e; ++b)
		node.ins.push_back(read_node(*b, group));

	sort( node.ins.begin(), node.ins.end() );
	return node;
}

void read_groups(nlohmann::json const &root)
{
	using namespace nlohmann;

	json jgrs = root["groups"];
	for(auto b = jgrs.begin(), e = jgrs.end(); b != e; ++b)
		groups.insert({ (string)(*b)["name"], tocolor(b->value("color", "f")) });
	return;
}

void init_dia()
{
	using namespace nlohmann;

	try
	{
		json root;
		cin >> root;
		read_groups(root);
		dia.push(read_node(root));
	}
	catch(exception const &e)
	{
		cerr << e.what() << endl;
		exit(0);
	}

	dia.calculate_bounds();
	rat = (vmode.width - 2*margin) / (double)(dia.max - dia.min);

	return;
}



/*
 * draw functions
 */
void draw_all()
{
	draw_dia();
	draw_scale();
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
		setCenteredPosition(text, x + w/2, y + h/2);

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

	Line line;
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





// END
