/* --------------------------------
   Author:           Alex Koukoulas
   Date:             13/12/2015
   File name:        dotmmain.cpp
   
   File description: Contains the
   custom window proc function and
   the main game loop
   -------------------------------- */

#if defined(DEBUG) || defined(_DEBUG)
#include "vld.h"
#endif

#include "window.h"
#include "states/gsqueue.h"
#include "rendering/renderer.h"
#include "handlers/inputhandler.h"

/* -------
   Globals
   ------- */
Window* g_window;
   
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
	Window window("winconfig",
				  hInstance,
				  messageHandler);

    // Assert window is ready
    if (!window.isReady()) return 1;
    g_window = &window;

	MSG message = {};
	GameStateQueue gsq;

    // Initialize Singletons
    Renderer::get();
    InputHandler::get();

	for (;;)
	{
		if (PeekMessage(&message, window.getHandle(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			if (message.message == WM_QUIT) break;
		}
		else
		{
			gsq.update();
			if (gsq.isDone()) break;
            gsq.render();
            InputHandler::get()->endFrame();
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

        case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (wparam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}            
		} break;
	}

    InputHandler::get()->handleInput(message, wparam, lparam);

	return DefWindowProc(windowHandle, message, wparam, lparam);
}