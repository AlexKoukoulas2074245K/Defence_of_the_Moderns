/* -------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        strings.cpp

   File description: Implementation of
   the functinos declared in strings.h
   ------------------------------------------- */

#include "strings.h"
#include "../dotmdef.h"
#include <unordered_map>
#include <functional>
#include <string>

/* -------------
   Global Vars
   ------------- */
static std::unordered_map<stringID, std::string> stringTable;
static std::hash<std::string> hashFunc;

/* ----------------
   Public Functions
   ---------------- */
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
    std::string str(std::to_string((uint32)number));
    return internString(str.c_str());
}

cstring 
retrieveString(const stringID id)
{
    if (stringTable.count(id)) return stringTable[id].c_str();
    return nullptr;
}