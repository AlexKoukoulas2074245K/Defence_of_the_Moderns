/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        window.h

   File description: A class representing
   a window adhering to the win32 api rules.
   The class loads and extracts the initial
   data from the configuration file parameter
   in the constructor
   ------------------------------------------ */

#pragma once

#include <Windows.h>
#include "dotmdef.h"
#include "util/strings.h"

class Window
{
public:

    Window(const char* configPath,
           const HINSTANCE& hInstance,
           const WNDPROC& windowProc);

    ~Window();

    Window(const Window& rhs) = delete;

    Window&
    operator = (const Window& rhs) = delete;

    bool 
    isReady() logical_const;

    bool
    isFullscreen() logical_const;

    uint32
    getWidth() logical_const;

    uint32
    getHeight() logical_const;

    real32
    getAspect() logical_const;

    cstring 
    getAppName() logical_const;

    const HWND&
    getHandle() logical_const;

private:

    static const uint32 WINDOW_DEFAULT_WD_WIDTH;

private:

    bool m_ready;
    bool m_fullscreen;

    HWND     m_handle;
    stringID m_appName;

    uint32 m_windowWidth;
    uint32 m_windowHeight;
    uint32 m_screenWidth;
    uint32 m_screenHeight;
    real32 m_aspectRatio;
};
