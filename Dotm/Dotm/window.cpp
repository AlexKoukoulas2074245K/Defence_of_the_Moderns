/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        window.cpp
   
   File description: A class representing
   a window adhering to the win32 api rules.
   The class loads and extracts the initial 
   data from the configuration file parameter
   in the constructor
   ------------------------------------------ */

#include "window.h"

/* -----------------
   Const Definitions
   ----------------- */
const uint32 Window::WINDOW_DEFAULT_WD_WIDTH = 1000U;

   
/* -------------------
   Internal Signatures 
   ------------------- */
internal bool
getBoolBuffer(const cstring buffer);

/* --------------
   Public Methods
   -------------- */
Window::Window(const cstring configPath,
			   const HINSTANCE& hInstance,
			   const WNDPROC& windowProc):

	ready(false),
	fullscreen(false),
	windowWidth(0),	
	windowHeight(0),
	handle(NULL)
{

	// Exctract screen size
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	aspectRatio  = static_cast<float>(screenWidth) / 
		           static_cast<float>(screenHeight);

	// Extract the app name from the winconfig file
	char appNameBuffer[32];
	GetPrivateProfileString("winconfig",
							"app_name",
							"",
							appNameBuffer,
							ARRAYSIZE(appNameBuffer),
							configPath);

	// Extract the fuillscreen boolean from the winconfig file
	char fullscreenBuffer[8];
	GetPrivateProfileString("winconfig",
							"fullscreen",
							"",
							fullscreenBuffer,
							ARRAYSIZE(fullscreenBuffer),
							configPath);

	fullscreen = getBoolBuffer(fullscreenBuffer);

	// Extract the vsync boolean from the winconfig file
	char vsyncBuffer[8];
	GetPrivateProfileString("winconfig",
							"vsync",
							"",
							vsyncBuffer,
							ARRAYSIZE(vsyncBuffer),
							configPath);

	vsync = getBoolBuffer(vsyncBuffer);

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

	if (fullscreen)
	{
		windowWidth  = screenWidth;
		windowHeight = screenHeight;

		// Display device info
		DEVMODE	dmScreen = {};
		dmScreen.dmSize = sizeof(dmScreen);
		dmScreen.dmPelsWidth = (ulong64) windowWidth;
		dmScreen.dmPelsHeight = (ulong64) windowHeight;
		dmScreen.dmBitsPerPel = 32;
		dmScreen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreen, CDS_FULLSCREEN);
	}
	else
	{
		windowWidth  = WINDOW_DEFAULT_WD_WIDTH;
		windowHeight = static_cast<uint32>(WINDOW_DEFAULT_WD_WIDTH /
										   aspectRatio);
	    // Center window
		windowPosX   = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
		windowPosY   = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	}

	// Window Creation
	handle = CreateWindowEx(NULL,
							appNameBuffer,
							appNameBuffer,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							windowPosX,
							windowPosY,
							windowWidth,
							windowHeight,
							NULL,
							NULL,
							hInstance,
							NULL);

	ShowWindow(handle, SW_SHOW);
	ShowCursor(false);
	ready = true;
}

Window::~Window()
{

}

bool 
Window::isReady() const
{
	return ready;
}

bool
Window::getFullscreen() const
{
	return fullscreen;
}

bool
Window::getVsync() const
{
	return vsync;
}

uint32
Window::getWidth() const
{
	return windowWidth;
}

uint32
Window::getHeight() const
{
	return windowHeight;
}

real32
Window::getAspect() const
{
	return aspectRatio;
}

const HWND&
Window::getHandle() const
{
	return handle;
}

/* ------------------
   Internal Functions
   ------------------ */
internal bool
getBoolBuffer(const cstring buffer)
{
	return buffer[0] == 't' &&
		   buffer[1] == 'r' &&
		   buffer[2] == 'u' &&
		   buffer[3] == 'e';
}