/* -------------------------------------------------
   Author:           Alex Koukoulas
   Date:             17/1/2016
   File name:        command.cpp
   
   File description: Implementations of the declared
   classes in command.h
   ------------------------------------------------- */

#include "command.h"
#include "../util/math.h"

/* ==============
   Class: Command
   ============== */
Command::Command(Entity* entity):

                 m_entity(entity)
{

}

Command::~Command()
{
}


/* ==================
   Class: MoveCommand
   ================== */
MoveCommand::MoveCommand(const vec3f& to, 
                         Entity* entity):
    
                         Command(entity),
                         m_targetPos(to)
{

}

MoveCommand::~MoveCommand()
{
}

void
MoveCommand::execute()
{
    m_entity->setTargetPos(m_targetPos);    
    delete this;
}

/* ==================
   Class: MoveCommand
   ================== */
DamageCommand::DamageCommand(const int32 damage,
                             Entity* entity):

                             Command(entity),
                             m_damage(damage)
{
}

DamageCommand::~DamageCommand()
{

}

void
DamageCommand::execute()
{
    m_entity->damage(m_damage);
    delete this;
}