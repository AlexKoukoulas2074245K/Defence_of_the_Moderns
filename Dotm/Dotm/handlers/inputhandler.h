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

private:

    InputHandler();

    InputHandler(const InputHandler& rhs) = delete;

    InputHandler& operator = (const InputHandler& rhs) = delete;

public:

    // Keyboard Constants
    static const uint32 KEY_SPACE = 0x001;
    static const uint32 KEY_LEFT = 0x002;
    static const uint32 KEY_UP = 0x004;
    static const uint32 KEY_RIGHT = 0x008;
    static const uint32 KEY_DOWN = 0x010;
    static const uint32 KEY_A = 0x020;
    static const uint32 KEY_W = 0x040;
    static const uint32 KEY_D = 0x080;
    static const uint32 KEY_S = 0x100;

    // Mouse Constants
    static const uint32 BUTTON_LEFT = 0x200;
    static const uint32 BUTTON_RIGHT = 0x400;
    static const uint32 BUTTON_MIDDLE = 0x800;

private:

    static const uint32 MOUSE_WHEEL_MID_POINT = 1024U;

private:

    std::unordered_map<uint32, uint32> m_recognizedInput;

    uint32 m_currState;
    uint32 m_prevState;
    int32  m_wheelDelta;

};