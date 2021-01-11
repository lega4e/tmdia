#ifndef LIS_MESCAL_HPP_89301
#define LIS_MESCAL_HPP_89301

#include <codecvt>
#include <string>





namespace lis
{





inline int hex2dec(char ch)
{
	return
		ch >= '0' && ch <= '9' ? ch - '0' :
		ch >= 'a' && ch <= 'z' ? 10 + ch - 'a' :
		ch >= 'A' && ch <= 'Z' ? 10 + ch - 'A' : -1;
}

inline std::wstring stows(std::string const &s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s.data());
}





}





#endif // LIS_MESCAL_HPP_89301
