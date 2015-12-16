/* -------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        strings.cpp

   File description: Implementation of
   the functinos declared in strings.h
   ------------------------------------------- */

#include "strings.h"
#include <map>
#include <functional>
#include <string>

/* -------------
   Global Vars
   ------------- */
std::map<stringID, std::string> stringTable;
std::hash<std::string> hashFunc;

/* --------------
   Public Methods
   -------------- */
stringID
internString(cstring str)
{
    std::string internalString = std::string(str);
    stringID id = hashFunc(internalString);
    stringTable[id] = internalString;
    return id;
}

stringID
internString(cwstring str)
{
    std::wstring wstr(str);
    std::string  s(wstr.begin(), wstr.end());
    return internString(s.c_str());
}

stringID
internString(void* number)
{
    std::string str(std::to_string((unsigned int)number));
    return internString(str.c_str());
}

cstring
retrieveString(const stringID id)
{
    if (stringTable.count(id)) return stringTable[id].c_str();
    return NULL;
}