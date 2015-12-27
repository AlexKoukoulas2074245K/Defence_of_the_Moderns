/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             22/12/2015
   File name:        inputhandler.h

   File description: A singleton class representing
   the main keyboard and mouse input handler.
   ------------------------------------------------ */

#pragma once

#include "../dotmdef.h"
#include "../window.h"
#include "../util/math.h"
#include <unordered_map>

class InputHandler
{
public:

    static InputHandler&
    get();

    ~InputHandler();

    void
    handleInput(const uint32 msg,
                const WPARAM wparam,
                const LPARAM lparam);

    void
    endFrame();

    bool
    isPressed(const uint32 code) logical_const;

    bool
    isTapped(const uint32 code) logical_const;

    int32
    getWheelDelta() logical_const;
    
    const vec2f&
    getMousePos() logical_const;

private:

    InputHandler();

    InputHandler(const InputHandler& rhs) = delete;

    InputHandler& operator = (const InputHandler& rhs) = delete;

public:

    // Keyboard Constants
    static const uint32 KEY_SPACE = 0x0001;
    static const uint32 KEY_LEFT  = 0x0002;
    static const uint32 KEY_UP    = 0x0004;
    static const uint32 KEY_RIGHT = 0x0008;
    static const uint32 KEY_DOWN  = 0x0010;
    static const uint32 KEY_A     = 0x0020;
    static const uint32 KEY_W     = 0x0040;
    static const uint32 KEY_D     = 0x0080;
    static const uint32 KEY_S     = 0x0100;
    static const uint32 KEY_Q     = 0x0200;
    static const uint32 KEY_E     = 0x0400;

    // Mouse Constants
    static const uint32 BUTTON_LEFT   = 0x0800;
    static const uint32 BUTTON_RIGHT  = 0x1000;
    static const uint32 BUTTON_MIDDLE = 0x2000;

private:

    // An arbitrary value barier seperating the
    // negative and positive mouse wheel spins
    static const uint32 MOUSE_WHEEL_MID_POINT = 1024U;

private:

    std::unordered_map<uint32, uint32> m_recognizedInput;

    uint32   m_currState;
    uint32   m_prevState;
    int32    m_wheelDelta;
    bool     m_mouseLocked;
    vec2f    m_mousePos;    
};