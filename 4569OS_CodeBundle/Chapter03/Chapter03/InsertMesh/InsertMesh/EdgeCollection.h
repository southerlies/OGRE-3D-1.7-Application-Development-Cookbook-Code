#pragma once
#include "Edge.h"
// CEdgeCollection command target

class CEdgeCollection : public CObject
{
public:
	CEdgeCollection();
	virtual ~CEdgeCollection();
	CObArray m_Collection;
};


