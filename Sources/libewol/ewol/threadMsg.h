/**
 *******************************************************************************
 * @file threadMsg.h
 * @brief User abstraction for message to a specific thread (Header)
 * @author Edouard DUPIN
 * @date 26/01/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */



#ifndef __EWOL_TREAD_MSG_H__
#define __EWOL_TREAD_MSG_H__

#include <pthread.h>

namespace ewol {
	namespace threadMsg {
		typedef enum {
			MSG_PRIO_REAL_TIME,
			MSG_PRIO_HIGH,
			MSG_PRIO_MEDIUM,
			MSG_PRIO_LOW,
			MSG_PRIO_NONE,
			MSG_PRIO_NUMBER,
		} msgPriority_te;
		
		#define NUMBER_OF_ELEMENT_IN_THE_FIFO        (1024)
		#define MAX_MSG_DATA_SIZE                    (128)
		typedef struct {
			bool           isActive;
			uint32_t       type;
			char           data[MAX_MSG_DATA_SIZE];
		} threadMsgContent_ts;
		
		
		typedef struct {
			bool                isInit;
			pthread_mutex_t     mutex;
			pthread_cond_t      condition;
			//etk::VectorType<threadMsgContent_ts>     
			threadMsgContent_ts listOfMessages[MSG_PRIO_NUMBER][NUMBER_OF_ELEMENT_IN_THE_FIFO];
			int32_t             nbMessages[MSG_PRIO_NUMBER];
			bool                displayHasDone;
		} threadMsg_ts;
		
		void Init(threadMsg_ts& messageData);
		void UnInit(threadMsg_ts& messageData);
		bool WaitMessage(threadMsg_ts& messageData, threadMsgContent_ts &data);
		int32_t WaitingMessage(threadMsg_ts& messageData);
		bool HaveMessage(threadMsg_ts& messageData);
		bool SendMessage(threadMsg_ts& messageData, uint32_t type, msgPriority_te prio = MSG_PRIO_NONE, void * data = NULL, uint32_t size = 0);
		void SendDisplayDone(threadMsg_ts& messageData);
		bool HasDisplayDone(threadMsg_ts& messageData);
	};
	
	namespace simpleMsg {
		typedef struct {
			bool                isInit;
			pthread_mutex_t     mutex;
			pthread_cond_t      condition;
			uint32_t            messageValue;
		} simpleMsg_ts;
		
		void     Init(          simpleMsg_ts& handle);
		void     UnInit(        simpleMsg_ts& handle);
		uint32_t WaitingMessage(simpleMsg_ts& handle, int32_t timeOut=0);
		void     SendMessage(   simpleMsg_ts& handle, uint32_t message);
		void     Clear(         simpleMsg_ts& handle);
	};
};

#endif

