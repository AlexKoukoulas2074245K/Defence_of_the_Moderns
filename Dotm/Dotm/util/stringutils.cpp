/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             19/12/2015
   File name:        stringutils.cpp

   File description: Implementation of the
   string utility functions declared in
   stringutils.h
   ------------------------------------------ */

#include "stringutils.h"
#include <sstream>

std::wstring
string_utils::getwstring(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

std::string
string_utils::getstring(const std::wstring& wstr)
{
    return std::string(wstr.begin(), wstr.end());
}

void
string_utils::split(const std::string& s,
                    char delim,
                    std::vector<std::string>& elems)
{
    std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) 
    {
		elems.push_back(item);
	}	
}

std::vector<std::string>
string_utils::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}