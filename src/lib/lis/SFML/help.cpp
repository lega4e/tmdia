#include <lis/SFML/help.hpp>

#include <lis/mescal.hpp>
#include <stdexcept>





namespace lis
{





sf::Color tocolor(std::string const &s)
{
	if(s[0] == '#')
		return tocolor(s.substr(1));

	switch(s.size())
	{
	case 1:	
	  {
		int tmp = hex2dec(s[0]);
		tmp = tmp * 16 + tmp;
		return sf::Color(tmp, tmp, tmp);
	  }
	case 3: case 4:
	  {
		int r = hex2dec(s[0]);
		r = r * 16 + r;

		int g = hex2dec(s[1]);
		g = g * 16 + g;

		int b = hex2dec(s[2]);
		b = b * 16 + b;

		int a = 255;
		if(s.size() == 4u)
			a = hex2dec(s[3]),
			a = a * 16 + a;

		return sf::Color(r, g, b, a);
	  }
	case 6: case 8:
		return sf::Color(
			hex2dec(s[0])*16 + hex2dec(s[1]),
			hex2dec(s[2])*16 + hex2dec(s[3]),
			hex2dec(s[4])*16 + hex2dec(s[5]),
			s.size() == 6 ? 255 :
			hex2dec(s[5])*16 + hex2dec(s[7])
		);
	default:
		throw std::invalid_argument("tocolor: invalid color '" + s + "'");
	}
}





}





// end
