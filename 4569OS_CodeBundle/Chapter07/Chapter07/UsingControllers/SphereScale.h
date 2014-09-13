#pragma once

#include "Ogre.h"
using namespace Ogre;

class SphereScale :	public ControllerValue<Real>
{
public:

	SphereScale(Node *Node, Real Scale)
{
	m_Node = Node;
	m_Scale = Scale;
}

Real getValue(void) const
{
	return m_Scale;
}

void setValue(Real value)
{
	m_Scale = (value / m_Scale) + 0.5;
	m_Node->scale(m_Scale, m_Scale, m_Scale);
}
		
	Node *m_Node;
	Real m_Scale;
};

