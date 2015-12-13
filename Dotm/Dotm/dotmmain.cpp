/* --------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        dotmmain.cpp
   
   File description: Contains the
   custom window proc function and
   the main game loop
   -------------------------------- */

#include "window.h"
#include "vld.h"
#include "strings.h"

/* -------------------
   Internal Signatures 
   ------------------- */
LRESULT CALLBACK 
messageHandler(HWND windowHandle,
			   UINT message,
			   WPARAM wparam,
			   LPARAM lparam);			   

/* ----------------
   Main Entry Point
   ---------------- */
int CALLBACK 
WinMain(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow)
{
	Window window("config/winconfig.ini",
				  hInstance,
				  messageHandler);
	
	MSG message = {};
	for (;;)
	{
		if (PeekMessage(&message, window.getHandle(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			if (message.message == WM_QUIT) break;
		}
	}

	return EXIT_SUCCESS;
}

/* ------------------
   Internal Functions
   ------------------ */
LRESULT CALLBACK 
messageHandler(HWND windowHandle,
			   UINT message,
			   WPARAM wparam,
			   LPARAM lparam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;

		case WM_KEYDOWN:
		{
			if (wparam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
		} break;
	}

	return DefWindowProc(windowHandle, message, wparam, lparam);
}