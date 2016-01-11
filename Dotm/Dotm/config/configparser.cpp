/* -----------------------------------------------
   Author:           Alex Koukoulas
   Date:             16/12/2015
   File name:        configparser.cpp
   
   File description: An implementation of the
   configparser methods declared in configparser.h
   ----------------------------------------------- */

#include "configparser.h"
#include "../window.h"
#include <sys\stat.h>
#include <string>

/* -------
   Globals
   ------- */
static std::string i_configFileName;

/* -------------------
   Internal Signatures
   ------------------- */
static bool
getBoolBuffer(cstring buffer);

static int
getBufferInt(cstring charBuffer);

static float
getBufferFloat(cstring charBuffer);

/* --------------
   Public Methods
   -------------- */
bool
config::initConfigFile(cstring configName)
{
    std::string fullPath(configName);
    fullPath = "config/" + fullPath + ".ini";

    struct stat buffer;
    if (stat(fullPath.c_str(), &buffer))
    {
        std::string message("The ");
        message += configName;
        message += " config file could not be found. Press OK to exit the application";
        MessageBox(nullptr, message.c_str(), "Config file not Found", MB_ICONERROR);
        return false;
    }

    i_configFileName = fullPath;
    return true;
}

void
config::extractConfigString(cstring   section,
                            cstring   varname,
                            stringID* outStringID)
{
    char buffer[32];
    GetPrivateProfileString(section,
                            varname,
                            "",
                            buffer,
                            ARRAYSIZE(buffer),
                            i_configFileName.c_str());

    *outStringID = internString(buffer);
}

void
config::extractConfigBool(cstring section,
                          cstring varname,
                          bool*   outBool)
{
    char buffer[8];
    GetPrivateProfileString(section,
                            varname,
                            "",
                            buffer,
                            ARRAYSIZE(buffer),
                            i_configFileName.c_str());

    *outBool = getBoolBuffer(buffer);
}

void 
config::extractConfigUint(cstring section,
                          cstring varname,
                          uint32* outUint)
{
    char buffer[16];
    GetPrivateProfileString(section,
		                    varname,
		                    "",
		                    buffer,
		                    ARRAYSIZE(buffer),
                    		i_configFileName.c_str());

    *outUint = getBufferInt(buffer);
}

void
config::extractConfigFloat(cstring section,
                           cstring varname,
                           float*  outFloat)
{
    char buffer[16];
    GetPrivateProfileString(section,
		                    varname,
		                    "",
		                    buffer,
		                    ARRAYSIZE(buffer),
                    		i_configFileName.c_str());

    *outFloat = getBufferFloat(buffer);
}

/* ------------------
   Internal Functions
   ------------------ */
static bool
getBoolBuffer(cstring buffer)
{
    return buffer[0] == 't' &&
           buffer[1] == 'r' &&
           buffer[2] == 'u' &&
           buffer[3] == 'e';
}

int
getBufferInt(cstring charBuffer)
{
    return atoi(charBuffer);
}

float
getBufferFloat(cstring charBuffer)
{
    return (float) atof(charBuffer);
}