/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        inputhandler.h
   
   File description: Implementation of the 
   InputHandler class declared in inputhandler.h. 
   ------------------------------------------------ */

#include "inputhandler.h"
#include "../window.h"
#include "../util/logging.h"

/* -------------
   External Vars
   ------------- */
extern Window* g_window;

/* --------------
   Public Methods
   -------------- */
InputHandler*
InputHandler::get()
{
    static InputHandler instance;
    return &instance;
}

InputHandler::~InputHandler()
{

}

void
InputHandler::handleInput(const uint32 msg,
                          const WPARAM wparam,
                          const LPARAM lparam)
{
    switch (msg)
    {
        // Key down events
        case WM_KEYDOWN:
        {
            if (m_recognizedInput.count(wparam))
            {
                m_currState |= m_recognizedInput[wparam];
            }
        } break;
        
        // Key up events
        case WM_KEYUP:
        {
            if (m_recognizedInput.count(wparam))
            {
                m_currState ^= m_recognizedInput[wparam];
            }
        } break;

        // Mouse Move event
        case WM_MOUSEMOVE:
        {
            POINT mousePos = {};
            GetCursorPos(&mousePos);
            ScreenToClient(g_window->getHandle(), &mousePos);
            
            m_mousePos.x = (real32) mousePos.x;
            m_mousePos.y = (real32) mousePos.y;
        } break;

        // Mouse wheel rotatation event
        case WM_MOUSEWHEEL:
        {
            // Extract the High Word from wparam
            int16 wheelResult = (wparam >> 16) & 0xFFFF;

            // Suppress added wheel movements
            m_wheelDelta = wheelResult < 0 ? -1 : 1;            
        } break;

        // Mouse Button down events
        case WM_LBUTTONDOWN: m_currState |= BUTTON_LEFT; break;
        case WM_RBUTTONDOWN: m_currState |= BUTTON_RIGHT; break;
        case WM_MBUTTONDOWN: m_currState |= BUTTON_MIDDLE; break;

        // Mouse Button up events
        case WM_LBUTTONUP: m_currState ^= BUTTON_LEFT; break;
        case WM_RBUTTONUP: m_currState ^= BUTTON_RIGHT; break;
        case WM_MBUTTONUP: m_currState ^= BUTTON_MIDDLE; break;          
    }
}

void
InputHandler::endFrame()
{
    m_prevState  = m_currState;
    m_wheelDelta = 0U;
}

// In the next two methods explicit checks
// against zero are done to avoid compile warnings
bool
InputHandler::isPressed(const uint32 code) logical_const
{
    return (m_currState & code) != 0;
}

bool
InputHandler::isTapped(const uint32 code) logical_const
{
    return isPressed(code) && (m_prevState & code) == 0;
}

int32
InputHandler::getWheelDelta() logical_const
{
    return m_wheelDelta;
}

const vec2f&
InputHandler::getMousePos() logical_const
{
    return m_mousePos;
}

/* ---------------
   Private Methods
   --------------- */
InputHandler::InputHandler():

    m_currState(0U),
    m_prevState(0U),
    m_wheelDelta(0U),
    m_mousePos()    
{
    m_recognizedInput[VK_SPACE] = KEY_SPACE;
    m_recognizedInput[VK_LEFT]  = KEY_LEFT;
    m_recognizedInput[VK_UP]    = KEY_UP;
    m_recognizedInput[VK_RIGHT] = KEY_RIGHT;
    m_recognizedInput[VK_DOWN]  = KEY_DOWN;
    m_recognizedInput[0x41]     = KEY_A;
    m_recognizedInput[0x57]     = KEY_W;
    m_recognizedInput[0x44]     = KEY_D;
    m_recognizedInput[0x53]     = KEY_S;
    m_recognizedInput[0x45]     = KEY_E;
    m_recognizedInput[0x51]     = KEY_Q;
}