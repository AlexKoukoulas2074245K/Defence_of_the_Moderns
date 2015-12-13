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

#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include "dotmdef.h"

class Window
{
public:

	Window(const char* configPath,
		   const HINSTANCE& hInstance,
		   const WNDPROC& windowProc);
   
   ~Window();
	
    Window(const Window& rhs) = delete;
	
	Window&
	operator= (const Window& rhs) = delete;

	bool
	isReady() const;

	bool
	getFullscreen() const;

	bool 
	getVsync() const;

	uint32
    getWidth() const;

	uint32
	getHeight() const;

	real32
	getAspect() const;

	const HWND&
	getHandle() const;
	
private:

	classfield const uint32 WINDOW_DEFAULT_WD_WIDTH;

private:

	bool ready;
	bool fullscreen;
	bool vsync;

	HWND handle;
	
	uint32 windowWidth;
	uint32 windowHeight;
	uint32 screenWidth;
	uint32 screenHeight;
	real32 aspectRatio;
};

#endif