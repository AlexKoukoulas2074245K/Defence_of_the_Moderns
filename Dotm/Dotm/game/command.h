/* ------------------------------------------------
   Author:           Alex Koukoulas
   Date:             17/1/2016
   File name:        command.h
   
   File description: A set of general purpose 
   command classes operating on individual entities
   (Command Pattern)
   ------------------------------------------------ */

#pragma once
#include "entity.h"
#include "../util/math.h"

/* ==============
   Class: Command
   ============== */
class Command
{
public:

    Command(Entity* entity);
    
    Command(const Command& rhs) = delete;

    Command&
    operator = (const Command& rhs) = delete;

    virtual
    ~Command() = 0;

    virtual void
    execute() = 0;

protected:

    Entity* m_entity;

};

/* ==================
   Class: MoveCommand
   ================== */
class MoveCommand: public Command
{
public:
    
    MoveCommand(const vec3f& to,
                Entity* entity);

    ~MoveCommand();

    void
    execute() override;

private:

    vec3f m_targetPos;

};
