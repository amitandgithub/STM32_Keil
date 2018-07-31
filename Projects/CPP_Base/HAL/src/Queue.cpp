/*
 * Queue.cpp
 *
 *  Created on: 22-Oct-2017
 *      Author: Amit Chaudhary
 */

#include <Queue.hpp>

namespace Bsp
{

Queue::Queue(QType* pQueueMem, u32 Size)
{
	m_Size = Size;
	m_pQueueMem = pQueueMem;

	if(m_pQueueMem)
	{
		for(u32 i=0;i<m_Size;i++)
		{
			m_pQueueMem[i] = 0;
		}
	}
	m_WriteHead = -1;
	m_ReadHead  = -1;

}

bool Queue::IsQueueFull()
{
	if( ( (m_WriteHead == 0) && (m_ReadHead == (signed)(m_Size-1) )) || (m_ReadHead == (m_WriteHead-1)) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Queue::IsQueueEmpty()
{
	if(m_WriteHead == -1)
	{
		return true;
	}
	return false;

}


Queue::QERROR Queue::Write(QType data )
{
	if(m_pQueueMem == nullptr)
	{
		return QNULLPOINTER;
	}
	else if(IsQueueFull())
	{
		return QFULL;
	}
	else
	{
		if(m_ReadHead == -1)
			m_ReadHead = 0;

		m_WriteHead = (m_WriteHead + 1) % m_Size;

		m_pQueueMem[m_WriteHead] = data;
	}
	return QSUCESS;
}

Queue::QERROR Queue::Read(QType &data )
{
	if(m_pQueueMem == nullptr)
	{
		return QNULLPOINTER;
	}
	else if(IsQueueEmpty())
	{
		return QEMPTY;
	}
	else
	{
		data = m_pQueueMem[m_WriteHead];

		if(m_ReadHead == m_WriteHead)
		{
			// Q has only one element, so we reset the queue
			m_ReadHead = m_WriteHead = -1;
		}
		else
		{
			m_ReadHead = (m_ReadHead + 1) % m_Size;
		}
	}
	return QSUCESS;
}

Queue::~Queue()
{

}


}// namespace Bsp
