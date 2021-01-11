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

#include "Axis.hpp"
#include "Grid.hpp"
#include "TimeDia.hpp"


using namespace lis;
using namespace std;
using namespace sf;





// types
/*
 * Структура, содержащая настройки интервала
 */
struct insets_type {
	Color color     = Color::Black;       // Цвет текста
	Color fillcolor = Color::Transparent; // Цвет закраски
	Color outcolor  = Color::Black;       // Цвет обводки
	float outthick  = 0;                  // Толщина обводки
	int   chsize    = 20;                 // Размер шрифта
};

typedef std::shared_ptr<insets_type> insets_ptr;

/*
 * Структура, которую будет хранить интервал:
 * строка текста, отображаемая в интервале
 * и его настройки
 */
typedef struct {
	std::wstring s;
	insets_ptr   sets;
} indata_type;

typedef lis::td::TimeDia<indata_type> dia_type;





// global objects
RenderWindow window;
VideoMode    vmode           = VideoMode::getDesktopMode();
char const   *TITLE          = "Application";
unsigned int FRAMERATE_LIMIT = 60u;
Color        bgcolor         = Color::White;



// objects
Font           font;
RectangleShape rect;
Text           text;
Axis           axis;
Grid           grid;

dia_type dia;
map<string, insets_ptr> groups;


int space  = 7;
int height = 40;
int margin = 75;
int step   = 50;
int tiplen = 20;
int tipth  = 3;
int axisth = 3;
int inmin  = int_max;
int inmax  = int_max;
Color tipcolor  = Color::Black;
Color axiscolor = Color::Black;

double rat = 0.0;





/*
 * Functions for read intervals from json file
 */
// Если текущий json-объект содержит отличающиеся
// настройки, то создать новый объект настроек на
// основе имеющегося и переопределить соответству-
// ющие элементы настройки
void override_sets(
	nlohmann::json const &j,
	insets_ptr &sets
)
{
	if(!sets)
		sets = groups["global"];

	if( j.find("color")     == j.end() &&
		j.find("fillcolor") == j.end() &&
		j.find("outcolor")  == j.end() &&
		j.find("outthick")  == j.end() &&
		j.find("chsize")    == j.end() )
	{
		return;
	}

	sets = insets_ptr(new insets_type( *sets ));

	if( j.find("color")     != j.end() ) sets->color     = tocolor( j["color"]     );
	if( j.find("fillcolor") != j.end() ) sets->fillcolor = tocolor( j["fillcolor"] );
	if( j.find("outcolor")  != j.end() ) sets->outcolor  = tocolor( j["outcolor"]  );
	if( j.find("outthick")  != j.end() ) sets->outthick  = j["outthick"];
	if( j.find("chsize")    != j.end() ) sets->chsize    = j["chsize"];

	return;
}

// Считать непосредственно интервал
td::INode<indata_type> read_leaf_node(
	nlohmann::json const &j,
	insets_ptr sets = nullptr
)
{
	using namespace nlohmann;

	override_sets(j, sets);
	td::Interval<indata_type> in;
	in.beg   = j["beg"];
	in.end   = j["end"];
	in.value = indata_type{ stows(j["label"]), sets };

	return td::INode<indata_type>(in);
}

// Считать интервал или групу интервалов
td::INode<indata_type> read_node(
	nlohmann::json const &j,
	insets_ptr sets = nullptr
)
{
	using namespace nlohmann;

	if(j.find("items") == j.end())
		return read_leaf_node(j, sets);

	td::INode<indata_type> node(false);

	if(j.find("group") != j.end())
		sets = groups[ j["group"] ];
	override_sets(j, sets);

	json items = j["items"];
	for(auto b = items.begin(), e = items.end(); b != e; ++b)
		node.ins.push_back(read_node(*b, sets));

	sort( node.ins.begin(), node.ins.end() );
	return node;
}

// Считать настройки групп
void read_groups(nlohmann::json const &root)
{
	using namespace nlohmann;

	json jgrs = root["groups"];
	insets_ptr sets = nullptr;

	/*
	 * Сначала необходимо считать глобальную группу,
	 * так как на её основе будут определяться
	 * все остальные
	 */
	for(auto b = jgrs.begin(), e = jgrs.end(); b != e; ++b)
	{
		if((*b)["name"] == "global")
		{
			sets = insets_ptr(new insets_type);
			override_sets( *b, sets );
			groups.insert( { "global", sets } );
			break;
		}
	}

	if(!sets)
		groups.insert( { "global", insets_ptr(new insets_type) } );

	for(auto b = jgrs.begin(), e = jgrs.end(); b != e; ++b)
	{
		if( (*b)["name"] == "global" )
			continue;
		sets = groups["global"];
		override_sets(*b, sets);
		groups.insert({ (string)(*b)["name"], sets });
	}

	return;
}




/*
 * Init functions
 */
void init()
{
	init_window();
	init_text();
	init_dia();
	init_axis_and_grid();
	return;
}

void init_window()
{
	window.create(vmode, TITLE, Style::Fullscreen);
	window.setPosition({0u, 0u});
	window.setFramerateLimit(FRAMERATE_LIMIT);
	return;
}

void init_text()
{
	font.loadFromFile("times.ttf");
	text.setFont(font);
	text.setStyle(Text::Bold);

	return;
}

void init_dia()
{
	try
	{
		nlohmann::json root;
		cin >> root;
		read_groups(root);
		dia.push(read_node(root));

		bgcolor   = tocolor(root.value("bgcolor", "f"));
		space     = root.value( "space",     space     );
		height    = root.value( "height",    height    );
		margin    = root.value( "margin",    margin    );
		step      = root.value( "step",      step      );
		tiplen    = root.value( "tiplen",    tiplen    );
		tipth     = root.value( "tipthick",  tipth     );
		axisth    = root.value( "axisthick", axisth    );
		inmin     = root.value( "min",       inmin     );
		inmax     = root.value( "max",       inmax     );
		tipcolor  = tocolor(root.value("tipcolor",  "f"));
		axiscolor = tocolor(root.value("axiscolor", "f"));
	}
	catch(exception const &e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	dia.calculate_bounds();
	if(inmin != int_max)
		dia.min = inmin;
	if(inmax != int_max)
		dia.max = inmax;

	rat = (vmode.width - 2*margin) / (double)(dia.max - dia.min);

	return;
}

void init_axis_and_grid()
{
	axis.pos( { 0, (float)margin/2 }, { (float)vmode.width, (float)margin/2 } ).
	axisThick(axisth).
	axisColor(axiscolor).
	tipStyle(Axis::middle).
	tipThick(tipth).
	tipColor(tipcolor).
	drawSign(true).
	font(font).
	text().setCharacterSize(tiplen);

	grid.size = makep( (float)vmode.width, (float)vmode.height );

	Axis::tips_type tips;

	for(
		int beg = dia.min % step == 0 ? dia.min : (dia.min / step + 1) * step;
		beg <= dia.max;
		beg += step
	)
	{
		tips.push_back( { margin + (beg - dia.min) * rat, to_string(beg) } );
		grid.verlines.push_back( margin + (beg - dia.min) * rat );
	}
	grid.line.color(tocolor("8"));

	axis.tips(tips);
	return;
}





/*
 * draw functions
 */
void draw_all()
{
	draw_grid();
	draw_dia();
	draw_axis();
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
		rect.setFillColor( b->value.sets->fillcolor );
		rect.setOutlineThickness( b->value.sets->outthick );
		rect.setOutlineColor( b->value.sets->outcolor );

		text.setString(b->value.s);
		text.setFillColor(b->value.sets->color);
		text.setCharacterSize(b->value.sets->chsize);
		setCenteredPosition(text, x + w/2, y + h/2);

		window.draw(rect);
		window.draw(text);
	}

	return;
}

void draw_axis()
{
	window.draw(axis);
	return;
}

void draw_grid()
{
	window.draw(grid);
	return;
}





// END
