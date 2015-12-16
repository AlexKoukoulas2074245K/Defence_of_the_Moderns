/* ------------------------------------
Author:           Alex Koukoulas
Date:             13/12/2015
File name:        strings.h

File description: Custom string
intern system. String IDs are
used instead of concrete strings
or char arrays throughout the system
------------------------------------ */

#pragma once

typedef size_t stringID;
typedef const char* cstring;
typedef const wchar_t* cwstring;

stringID
internString(cstring str);

stringID
internString(cwstring str);

stringID
internString(void* number);

cstring
retrieveString(const stringID id);
