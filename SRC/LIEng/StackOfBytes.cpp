#include "StackOfBytes.h"

namespace interpreter {

//------------------------------------------------------------------------------
CStackOfBytes::CStackOfBytes(void):
	m_dwAllocSize( sc_dwInitialStackSize ),
	m_dwUsedSize ( 0 )
{
	m_pRoot = new byte[ sc_dwInitialStackSize ];
}

//------------------------------------------------------------------------------
CStackOfBytes::~CStackOfBytes(void)
{
	delete[] m_pRoot;
}

//------------------------------------------------------------------------------
}