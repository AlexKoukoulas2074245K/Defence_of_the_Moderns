/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        window.cpp

   File description: Implementation of the
   Window class.
   ------------------------------------------ */

#include "window.h"
#include "config/configparser.h"

/* -----------------
   Const Definitions
   ----------------- */
const uint32 Window::WINDOW_DEFAULT_WD_WIDTH = 1000U;

/* --------------
   Public Methods
   -------------- */
Window::Window(const char* configPath,
               const HINSTANCE& hInstance,
               const WNDPROC& windowProc):

    m_ready(false),
    m_fullscreen(false),
    m_windowWidth(0),
    m_windowHeight(0),
    m_handle(NULL)
{

    // Exctract screen size
    m_screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    m_screenHeight = GetSystemMetrics(SM_CYSCREEN);
    m_aspectRatio  = (real32) m_screenWidth /
                     (real32) m_screenHeight;
    
    // assert config file exists
    if(!initConfigFile(configPath)) return;

    // Extract the app name from the winconfig file
    extractConfigString("winconfig", "app_name", &m_appName);
    
    // Extract the fuillscreen boolean from the winconfig file
    extractConfigBool("winconfig", "fullscreen", &m_fullscreen);

    // Fill out the window description struct
    WNDCLASSEX windowDesc    = {};
    windowDesc.cbSize        = sizeof(WNDCLASSEX);
    windowDesc.style         = CS_HREDRAW | CS_VREDRAW;
    windowDesc.lpfnWndProc   = windowProc;
    windowDesc.hInstance     = hInstance;
    windowDesc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    windowDesc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    windowDesc.lpszClassName = retrieveString(m_appName);

    RegisterClassEx(&windowDesc);

    uint32 windowPosX = 0U;
    uint32 windowPosY = 0U;

    if (m_fullscreen)
    {
        m_windowWidth  = m_screenWidth;
        m_windowHeight = m_screenHeight;

        // Display device info
        DEVMODE	dmScreen      = {};
        dmScreen.dmSize       = sizeof(dmScreen);
        dmScreen.dmPelsWidth  = (ulong64) m_windowWidth;
        dmScreen.dmPelsHeight = (ulong64) m_windowHeight;
        dmScreen.dmBitsPerPel = 32;
        dmScreen.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreen, CDS_FULLSCREEN);
    }
    else
    {
        m_windowWidth  = WINDOW_DEFAULT_WD_WIDTH;
        m_windowHeight = (uint32) (WINDOW_DEFAULT_WD_WIDTH / m_aspectRatio);

        // Center window
        windowPosX = (GetSystemMetrics(SM_CXSCREEN) - m_windowWidth) / 2;
        windowPosY = (GetSystemMetrics(SM_CYSCREEN) - m_windowHeight) / 2;
    }

    // Window Creation
    m_handle = CreateWindowEx(NULL,
                              retrieveString(m_appName),
                              retrieveString(m_appName),
                              WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
                              windowPosX,
                              windowPosY,
                              m_windowWidth,
                              m_windowHeight,
                              nullptr,
                              nullptr,
                              hInstance,
                              nullptr);

    ShowWindow(m_handle, SW_SHOW);
    ShowCursor(true);
    m_ready = true;
}

Window::~Window()
{

}

bool 
Window::isReady() logical_const
{
    return m_ready;
}

bool 
Window::isFullscreen() logical_const
{
    return m_fullscreen;
}

uint32 
Window::getWidth() logical_const
{
    return m_windowWidth;
}

uint32 
Window::getHeight() logical_const
{
    return m_windowHeight;
}

real32 
Window::getAspect() logical_const
{
    return m_aspectRatio;
}

cstring 
Window::getAppName() logical_const
{
    return retrieveString(m_appName);
}

const HWND&
Window::getHandle() logical_const
{
    return m_handle;
}