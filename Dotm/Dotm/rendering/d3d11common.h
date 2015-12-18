/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             15/12/2015
   File name:        d3d11common.h

   File description: A header file containing all
   the necessary linking, headers and common macros
   used for debugging HR bad results
   ------------------------------------------------ */

#pragma once

// Necessary header inclusion
#include <wrl\client.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxerr.h>

// Linking to external libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxerr.lib")

// D3D HR Debugging Macro
#if defined(DEBUG) | defined(_DEBUG)
    #ifndef HR
        #define HR(x)                                               \
        {                                                           \
            HRESULT hr = (x);                                       \
            if(FAILED(hr))                                          \
            {                                                       \
                DXTraceW(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
            }                                                       \
        }
    #endif // End of HR macro definition
#else // Debug Not defined
    #ifndef HR
        #define HR(x)(x)
    #endif
#endif // End of HR Debug Macro

// Com pointer convenience macro
#define comptr Microsoft::WRL::ComPtr
