#include <locale>

#include "declare.hpp"


using namespace sf;
using namespace std;





// main
int main( int argc, char *argv[] )
{
	std::setlocale(LC_ALL, "");
	init();

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

		window.clear(bgcolor);
		draw_all();
		window.display();
	}

	return 0;
}





// end
#include <lis/SFML/help.cpp>
