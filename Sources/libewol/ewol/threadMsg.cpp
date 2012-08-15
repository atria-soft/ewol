/**
 *******************************************************************************
 * @file threadMsg.cpp
 * @brief User abstraction for message to a specific thread (Sources)
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



#include <ewol/ewol.h>
#include <ewol/Debug.h>
#include <ewol/threadMsg.h>

void ewol::threadMsg::Init(ewol::threadMsg::threadMsg_ts& messageData)
{
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		messageData.nbMessages[iii] = 0;
		for (int32_t jjj=0; NUMBER_OF_ELEMENT_IN_THE_FIFO>jjj; jjj++) {
			messageData.listOfMessages[iii][jjj].isActive = false;
		}
	}
	messageData.isInit = true;
}

void ewol::threadMsg::UnInit(ewol::threadMsg::threadMsg_ts& messageData)
{
	if (true == messageData.isInit) {
		// remove data ????
		messageData.isInit = false;
	}
}

bool ewol::threadMsg::WaitMessage(ewol::threadMsg::threadMsg_ts& messageData, ewol::threadMsg::threadMsgContent_ts &data)
{
	if (false == messageData.isInit) {
		return false;
	}
	messageData.m_mutex.Lock();
	bool findAnOtherMessageInStack = false;
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		if (0 < messageData.nbMessages[iii]) {
			findAnOtherMessageInStack = true;
		}
	}
	if (false == findAnOtherMessageInStack) {
		messageData.m_mutex.UnLock();
		messageData.m_semaphore.Wait();
		messageData.m_mutex.Lock();
	}
	// find the message : 
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		if (0 < messageData.nbMessages[iii]) {
			// find a message : 
			if (false == messageData.listOfMessages[iii][0].isActive) {
				EWOL_ERROR( "an error occured ==> bad case ...");
				data.isActive = false;
				data.type = 0;
				data.data[0] = '\0';
			} else {
				data = messageData.listOfMessages[iii][0];
				messageData.listOfMessages[iii][0].isActive = false;
			}
			memmove(&messageData.listOfMessages[iii][0], &messageData.listOfMessages[iii][1], (NUMBER_OF_ELEMENT_IN_THE_FIFO-1)*sizeof(ewol::threadMsg::threadMsgContent_ts) );
			
			messageData.listOfMessages[iii][NUMBER_OF_ELEMENT_IN_THE_FIFO-1].isActive = false;
			/*
			for (int32_t jjj=0; NUMBER_OF_ELEMENT_IN_THE_FIFO>jjj; jjj++) {
				if (true == messageData.listOfMessages[iii][jjj].isActive) {
					// copy the data : 
					data = messageData.listOfMessages[iii][jjj];
					// disable the current message : 
					messageData.listOfMessages[iii][jjj].isActive = false;
				}
			}
			*/
			// decrement the number of message : 
			messageData.nbMessages[iii]--;
			if (0>messageData.nbMessages[iii]) {
				messageData.nbMessages[iii] = 0;
			}
			// exit the waiting system ...
			break;
		}
	}
	messageData.m_mutex.UnLock();
	return true;
}

bool ewol::threadMsg::SendMessage(ewol::threadMsg::threadMsg_ts& messageData, uint32_t type, ewol::threadMsg::msgPriority_te prio, void * data, uint32_t size)
{
	if (false == messageData.isInit) {
		return false;
	}
	if ( 0>prio || MSG_PRIO_NUMBER <= prio) {
		EWOL_ERROR("Send message with an unknown priority ... " << prio);
		return false;
	}
	if (size > MAX_MSG_DATA_SIZE) {
		EWOL_ERROR("Send message with an biger size than predictible " << size << " > " << MAX_MSG_DATA_SIZE);
		return false;
	}
	messageData.m_mutex.Lock();
	int32_t lastNbMessage = messageData.nbMessages[prio];
	for (int32_t jjj=0; NUMBER_OF_ELEMENT_IN_THE_FIFO>jjj; jjj++) {
		if (messageData.listOfMessages[prio][jjj].isActive == false) {
			// we find a slot ...
			messageData.listOfMessages[prio][jjj].isActive = true;
			messageData.listOfMessages[prio][jjj].type = type;
			memset(messageData.listOfMessages[prio][jjj].data, 0, MAX_MSG_DATA_SIZE*sizeof(char) );
			if (data!=NULL) {
				memcpy(messageData.listOfMessages[prio][jjj].data, data, size);
			}
			//EWOL_DEBUG("Find Slot : (" << prio << "," << jjj << ")");
			messageData.nbMessages[prio]++;
			break;
		}
	}
	/*
	EWOL_DEBUG("send message (" << messageData.nbMessages[MSG_PRIO_REAL_TIME] << ","
	                            << messageData.nbMessages[MSG_PRIO_HIGH] << ","
	                            << messageData.nbMessages[MSG_PRIO_MEDIUM] << ","
	                            << messageData.nbMessages[MSG_PRIO_LOW] << ","
	                            << messageData.nbMessages[MSG_PRIO_NONE] << ")");
	*/
	bool returnValue = false;
	if (lastNbMessage != messageData.nbMessages[prio]) {
		returnValue = true;
	} else {
		EWOL_ERROR("Send message Add error");
		returnValue = false;
	}
	messageData.m_semaphore.Post();
	messageData.m_mutex.UnLock();
	return returnValue;
}

int32_t ewol::threadMsg::WaitingMessage(threadMsg_ts& messageData)
{
	if (false == messageData.isInit) {
		return false;
	}
	messageData.m_mutex.Lock();
	int32_t nbMessage = 0;
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		nbMessage += messageData.nbMessages[iii];
	}
	messageData.m_mutex.UnLock();
	return nbMessage;
}

bool ewol::threadMsg::HaveMessage(threadMsg_ts& messageData)
{
	if (0 < WaitingMessage(messageData)) {
		return true;
	}
	return false;
}

void ewol::threadMsg::SendDisplayDone(threadMsg_ts& messageData)
{
	if (false == messageData.isInit) {
		return;
	}
	messageData.m_mutex.Lock();
	messageData.displayHasDone = true;
	messageData.m_semaphore.Post();
	messageData.m_mutex.UnLock();
	
}


bool ewol::threadMsg::HasDisplayDone(threadMsg_ts& messageData)
{
	if (false == messageData.isInit) {
		return false;
	}
	bool state = false;
	messageData.m_mutex.Lock();
	state = messageData.displayHasDone;
	messageData.displayHasDone = false;;
	messageData.m_mutex.UnLock();
	return state;
}



#include <sys/time.h>



void ewol::simpleMsg::Init(ewol::simpleMsg::simpleMsg_ts& handle)
{
	handle.messageValue = 0;
	handle.isInit = true;
}

void ewol::simpleMsg::UnInit(ewol::simpleMsg::simpleMsg_ts& handle)
{
	if (true == handle.isInit) {
		// remove data ????
		handle.isInit = false;
	}
}

uint32_t ewol::simpleMsg::WaitingMessage(ewol::simpleMsg::simpleMsg_ts& handle, int32_t timeOut)
{
	if (false == handle.isInit) {
		return 0;
	}
	handle.m_mutex.Lock();
	
	if (0 == handle.messageValue) {
		handle.m_mutex.UnLock();
		if (handle.m_semaphore.Wait(timeOut)==false) {
			return false;
		}
		handle.m_mutex.Lock();
	}
	// copy message
	int32_t messageCopy = handle.messageValue;
	// reset it ...
	handle.messageValue = 0;
	
	handle.m_mutex.UnLock();
	return messageCopy;
}

void ewol::simpleMsg::SendMessage(ewol::simpleMsg::simpleMsg_ts& handle, uint32_t message)
{
	if (false == handle.isInit) {
		return;
	}
	handle.m_mutex.Lock();
	handle.messageValue = message;
	handle.m_semaphore.Post();
	handle.m_mutex.UnLock();
}


void ewol::simpleMsg::Clear(simpleMsg_ts& handle)
{
	if (false == handle.isInit) {
		return;
	}
	handle.m_mutex.Lock();
	if (handle.messageValue !=0) {
		handle.m_mutex.UnLock();
		handle.m_semaphore.Wait();
		return;
	}
	handle.m_mutex.UnLock();
}



