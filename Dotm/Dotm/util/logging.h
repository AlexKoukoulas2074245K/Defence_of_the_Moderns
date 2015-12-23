/* ---------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        logging.h

   File description: A collection of methods
   used for the project's logging system
   --------------------------------------------- */

#pragma once

#if defined(_DEBUG)

#include <Windows.h>
#include <string>
#include "../dotmdef.h"

inline void 
logstring(const char* str) { OutputDebugString(str); }

inline void 
logline(const char* line) { logstring(line); logstring("\n"); }

inline void
logline(const real32 arg) { logline(std::to_string(arg).c_str()); }

inline void
logline(const uint32 arg) { logline(std::to_string(arg).c_str()); }

inline void
logline(const int32 arg) { logline(std::to_string(arg).c_str()); }

inline void
logline(const bool arg) { if (arg) logline("true"); else logline("false"); }

inline void
logvar(const real32 arg) { logstring(std::to_string(arg).c_str()); }

inline void
logvar(const uint32 arg) { logstring(std::to_string(arg).c_str()); }

inline void
logvar(const int32 arg) { logstring(std::to_string(arg).c_str()); }

inline void
logvar(const bool arg) { if (arg) logstring("true"); else logstring("false"); }

inline void
lognewline() { logline("\n"); }

#else

inline void 
logstring(const char* str) {}

inline void 
logline(const char* line) {}

inline void
logline(const real32 arg) {}

inline void
logline(const uint32 arg) {}

inline void
logline(const int32 arg) {}

inline void
logline(const bool arg) {}

inline void
logvar(const real32 arg) {}

inline void
logvar(const uint32 arg) {}

inline void
logvar(const int32 arg) {}

inline void
logvar(const bool arg) {}

inline void
lognewline() {}

#endif