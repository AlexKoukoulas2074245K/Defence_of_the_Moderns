/* ------------------------------------------
   Author:           Alex Koukoulas
   Date:             14/12/2015
   File name:        gsqueue.cpp
   
   File description: A class implementing the
   GameStateQueue class declared in gsqueue.h
   ------------------------------------------ */

#include "gsqueue.h"
#include "playstate.h"

/* --------------
   Public Methods
   -------------- */
GameStateQueue::GameStateQueue():
	
	m_done(false)
{
	m_states.push(new PlayState);
}

GameStateQueue::~GameStateQueue()
{
	// In case of premature instance deletion
	// make sure there are no memory leaks 
	while (!m_states.empty()) discardState();
}

void
GameStateQueue::update()
{
	m_states.front()->update();
	if (m_states.front()->isFinished()) discardState();
	if (m_states.empty()) m_done = true;
}

void
GameStateQueue::discardState()
{
	delete(m_states.front());
	m_states.pop();
}

/* ---------------
   Private Methods
   --------------- */
bool
GameStateQueue::isDone() const
{
    return m_done;
}