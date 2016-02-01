/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             19/12/2015
   File name:        stringutils.h

   File description: A file containing some
   handy methods when using the std strings
   ------------------------------------------ */

#pragma once

#include "strings.h"
#include <string>
#include <vector>

namespace string_utils
{
    std::string
    strcat(cstring a, cstring b);

    std::wstring
    getwstring(const std::string& str);

    std::string
    getstring(const std::wstring& wstr);

    void
    split(const std::string&        s,
          char                      delim,
          std::vector<std::string>& elems);

    std::vector<std::string>
    split(const std::string& s,
          char               delim);
}

