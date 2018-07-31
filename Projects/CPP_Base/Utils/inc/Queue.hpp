/*
 * Queue.hpp
 *
 *  Created on: 22-Oct-2017
 *      Author: Amit Chaudhary
 */

#ifndef APP_INC_QUEUE_HPP_
#define APP_INC_QUEUE_HPP_

#include "stm32f10x.h"

namespace Bsp
{

class Queue {
public:
	typedef u32 QType;
	typedef enum
	{
		QSUCESS = 0,
		QFULL,
		QEMPTY,
		QNULLPOINTER

	}QERROR;

	Queue(QType* m_pQueueMem, u32 Size);
	virtual ~Queue();
	QERROR Write(QType data);
	QERROR Read(QType &data);
	inline bool IsQueueFull();
	inline bool IsQueueEmpty();
private:
	QType* m_pQueueMem;
	s32 m_WriteHead;
	s32 m_ReadHead;
	u32 m_Size;
};

}// namespace Bsp
#endif /* APP_INC_QUEUE_HPP_ */
