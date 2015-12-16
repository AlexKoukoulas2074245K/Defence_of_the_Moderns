/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        window.cpp

   File description: Implementation of the
   Window class.
   ------------------------------------------ */

#include "window.h"

/* -----------------
   Const Definitions
   ----------------- */
const uint32 Window::WINDOW_DEFAULT_WD_WIDTH = 1000U;


/* -------------------
   Internal Signatures
   ------------------- */
static bool
getBoolBuffer(const char* buffer);

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

    m_aspectRatio  = static_cast<float>(m_screenWidth) /
                     static_cast<float>(m_screenHeight);

    // Extract the app name from the winconfig file
    char appNameBuffer[32];
    GetPrivateProfileString("winconfig",
                            "app_name",
                            "",
                            appNameBuffer,
                            ARRAYSIZE(appNameBuffer),
                            configPath);

    m_appName = internString(appNameBuffer);

    // Extract the fuillscreen boolean from the winconfig file
    char fullscreenBuffer[8];
    GetPrivateProfileString("winconfig",
                            "fullscreen",
                            "",
                            fullscreenBuffer,
                            ARRAYSIZE(fullscreenBuffer),
                            configPath);

    m_fullscreen = getBoolBuffer(fullscreenBuffer);

    // Extract the vsync boolean from the winconfig file
    char vsyncBuffer[8];
    GetPrivateProfileString("winconfig",
                            "vsync",
                            "",
                            vsyncBuffer,
                            ARRAYSIZE(vsyncBuffer),
                            configPath);

    m_vsync = getBoolBuffer(vsyncBuffer);

    // Fill out the window description struct
    WNDCLASSEX windowDesc    = {};
    windowDesc.cbSize        = sizeof(WNDCLASSEX);
    windowDesc.style         = CS_HREDRAW | CS_VREDRAW;
    windowDesc.lpfnWndProc   = windowProc;
    windowDesc.hInstance     = hInstance;
    windowDesc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    windowDesc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    windowDesc.lpszClassName = appNameBuffer;

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
        dmScreen.dmPelsWidth  = static_cast<ulong64>(m_windowWidth);
        dmScreen.dmPelsHeight = static_cast<ulong64>(m_windowHeight);
        dmScreen.dmBitsPerPel = 32;
        dmScreen.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreen, CDS_FULLSCREEN);
    }
    else
    {
        m_windowWidth  = WINDOW_DEFAULT_WD_WIDTH;
        m_windowHeight = static_cast<uint32>(WINDOW_DEFAULT_WD_WIDTH /
                                             m_aspectRatio);
        // Center window
        windowPosX = (GetSystemMetrics(SM_CXSCREEN) - m_windowWidth) / 2;
        windowPosY = (GetSystemMetrics(SM_CYSCREEN) - m_windowHeight) / 2;
    }

    // Window Creation
    m_handle = CreateWindowEx(NULL,
                              appNameBuffer,
                              appNameBuffer,
                              WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
                              windowPosX,
                              windowPosY,
                              m_windowWidth,
                              m_windowHeight,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

    ShowWindow(m_handle, SW_SHOW);
    ShowCursor(true);
    m_ready = true;
}

Window::~Window()
{

}

bool
Window::isReady() const
{
    return m_ready;
}

bool
Window::getFullscreen() const
{
    return m_fullscreen;
}

bool
Window::getVsync() const
{
    return m_vsync;
}

uint32
Window::getWidth() const
{
    return m_windowWidth;
}

uint32
Window::getHeight() const
{
    return m_windowHeight;
}

real32
Window::getAspect() const
{
    return m_aspectRatio;
}

cstring
Window::getAppName() const
{
    return retrieveString(m_appName);
}

const HWND&
Window::getHandle() const
{
    return m_handle;
}

/* ------------------
   Internal Functions
   ------------------ */
static bool
getBoolBuffer(const char* buffer)
{
    return buffer[0] == 't' &&
           buffer[1] == 'r' &&
           buffer[2] == 'u' &&
           buffer[3] == 'e';
}