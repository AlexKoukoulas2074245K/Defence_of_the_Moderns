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
static cstring i_configFileName;

/* -------------------
   Internal Signatures
   ------------------- */
static bool
getBoolBuffer(cstring buffer);

static int
getBufferInt(cstring charBuffer);

/* --------------
   Public Methods
   -------------- */
bool
initConfigFile(cstring configFileName)
{
    struct stat buffer;
    if (stat(configFileName, &buffer))
    {
        std::string message("The ");
        message += configFileName;
        message += " config file could not be found. Press OK to exit the application";
        MessageBox(NULL, message.c_str(), "Config file not Found", MB_ICONERROR);
        return false;
    }

    i_configFileName = configFileName;
    return true;
}

void
extractConfigString(cstring section,
                    cstring varname,
                    stringID* outStringID)
{
    char buffer[32];
    GetPrivateProfileString(section,
                            varname,
                            "",
                            buffer,
                            ARRAYSIZE(buffer),
                            i_configFileName);

    *outStringID = internString(buffer);
}

void
extractConfigBool(cstring section,
                  cstring varname,
                  bool* outBool)
{
    char buffer[8];
    GetPrivateProfileString(section,
                            varname,
                            "",
                            buffer,
                            ARRAYSIZE(buffer),
                            i_configFileName);

    *outBool = getBoolBuffer(buffer);
}

void
extractConfigUint(cstring section,
                  cstring varname,
                  uint32* outUint)
{
    char buffer[16];
    GetPrivateProfileString(section,
		                    varname,
		                    "",
		                    buffer,
		                    ARRAYSIZE(buffer),
                    		i_configFileName);

    *outUint = getBufferInt(buffer);
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