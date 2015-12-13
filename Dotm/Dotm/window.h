/* --------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        window.h
   
   File description:
   -------------------------------- */

#ifndef _WINDOW_H
#define _WINDOW_H

#include <Windows.h>
#include "dotmdef.h"

class Window
{
public:

	Window(const cstring configPath,
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