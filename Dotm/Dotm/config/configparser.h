/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        configparser.h
   
   File description: A class containing helper
   methods for parsing config files
   ----------------------------------------------- */

#pragma once

#include "../util/strings.h"

namespace config
{
    bool
    initConfigFile(cstring configName);

    void
    extractConfigString(cstring   section,
                        cstring   varname,
                        stringID* outStringID);

    void
    extractConfigBool(cstring section,
                      cstring varname,
                      bool*   outBool);

    void
    extractConfigUint(cstring       section,
                      cstring       varname,
                      unsigned int* outUint);

    void
    extractConfigFloat(cstring section,
                       cstring varname,
                       float*  outFloat);
}



