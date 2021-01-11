#ifndef DECLARE_HPP
#define DECLARE_HPP

#include <SFML/Graphics.hpp>





// global objects
extern sf::RenderWindow window;
extern sf::VideoMode vmode;
extern char const *TITLE;
extern unsigned int FRAMERATE_LIMIT;





// functions & procedures
void init();
void init_window();
void init_text();
void init_dia();

void draw_all();
void draw_dia();
void draw_scale();





#endif
