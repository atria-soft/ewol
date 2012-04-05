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
	// create interface mutex :
	int ret = pthread_mutex_init(&messageData.mutex, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Mutex ...");
	// create contition :
	ret = pthread_cond_init(&messageData.condition, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Condition ...");
	if (ret != 0) {
		ret = pthread_mutex_destroy(&messageData.mutex);
		EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
	} else {
		for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
			messageData.nbMessages[iii] = 0;
			for (int32_t jjj=0; NUMBER_OF_ELEMENT_IN_THE_FIFO>jjj; jjj++) {
				messageData.listOfMessages[iii][jjj].isActive = false;
			}
		}
	}
	messageData.isInit = true;
}

void ewol::threadMsg::UnInit(ewol::threadMsg::threadMsg_ts& messageData)
{
	if (true == messageData.isInit) {
		// Remove Mutex
		int ret = pthread_cond_destroy(&messageData.condition);
		EWOL_ASSERT(ret == 0, "Error destroying Condition ...");
		// Remove condition
		ret = pthread_mutex_destroy(&messageData.mutex);
		EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
		// remove data ????
		messageData.isInit = false;
	}
}

bool ewol::threadMsg::WaitMessage(ewol::threadMsg::threadMsg_ts& messageData, ewol::threadMsg::threadMsgContent_ts &data)
{
	if (false == messageData.isInit) {
		return false;
	}
	pthread_mutex_lock(&messageData.mutex);
	bool findAnOtherMessageInStack = false;
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		if (0 < messageData.nbMessages[iii]) {
			findAnOtherMessageInStack = true;
		}
	}
	if (false == findAnOtherMessageInStack) {
		pthread_cond_wait(&messageData.condition, &messageData.mutex);
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
	pthread_mutex_unlock(&messageData.mutex);
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
	pthread_mutex_lock(&messageData.mutex);
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
	pthread_cond_broadcast(&messageData.condition);
	pthread_mutex_unlock(&messageData.mutex);
	return returnValue;
}

int32_t ewol::threadMsg::WaitingMessage(threadMsg_ts& messageData)
{
	if (false == messageData.isInit) {
		return false;
	}
	pthread_mutex_lock(&messageData.mutex);
	int32_t nbMessage = 0;
	for (int32_t iii=0; MSG_PRIO_NUMBER>iii; iii++) {
		nbMessage += messageData.nbMessages[iii];
	}
	pthread_mutex_unlock(&messageData.mutex);
	return nbMessage;
}

void ewol::threadMsg::SendDisplayDone(threadMsg_ts& messageData)
{
	
}



#include <sys/time.h>



void ewol::simpleMsg::Init(ewol::simpleMsg::simpleMsg_ts& handle)
{
	// create interface mutex :
	int ret = pthread_mutex_init(&handle.mutex, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Mutex ...");
	// create contition :
	ret = pthread_cond_init(&handle.condition, NULL);
	EWOL_ASSERT(ret == 0, "Error creating Condition ...");
	if (ret != 0) {
		ret = pthread_mutex_destroy(&handle.mutex);
		EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
	} else {
		handle.messageValue = 0;
	}
	handle.isInit = true;
}

void ewol::simpleMsg::UnInit(ewol::simpleMsg::simpleMsg_ts& handle)
{
	if (true == handle.isInit) {
		// Remove Mutex
		int ret = pthread_cond_destroy(&handle.condition);
		EWOL_ASSERT(ret == 0, "Error destroying Condition ...");
		// Remove condition
		ret = pthread_mutex_destroy(&handle.mutex);
		EWOL_ASSERT(ret == 0, "Error destroying Mutex ...");
		// remove data ????
		handle.isInit = false;
	}
}

uint32_t ewol::simpleMsg::WaitingMessage(ewol::simpleMsg::simpleMsg_ts& handle, int32_t timeOut)
{
	if (false == handle.isInit) {
		return 0;
	}
	pthread_mutex_lock(&handle.mutex);
	
	if (0 == handle.messageValue) {
		if (timeOut == 0) {
			pthread_cond_wait(&handle.condition, &handle.mutex);
		} else {
			struct timeval now;
			struct timespec timeout;
			gettimeofday(&now, NULL);
			timeout.tv_sec = now.tv_sec + timeOut/1000;
			timeout.tv_nsec = now.tv_usec * 1000 + timeOut%1000;
			pthread_cond_timedwait(&handle.condition, &handle.mutex, &timeout);
		}
	}
	// copy message
	int32_t messageCopy = handle.messageValue;
	// reset it ...
	handle.messageValue = 0;
	
	pthread_mutex_unlock(&handle.mutex);
	return messageCopy;
}

void ewol::simpleMsg::SendMessage(ewol::simpleMsg::simpleMsg_ts& handle, uint32_t message)
{
	if (false == handle.isInit) {
		return;
	}
	pthread_mutex_lock(&handle.mutex);
	handle.messageValue = message;
	pthread_cond_broadcast(&handle.condition);
	pthread_mutex_unlock(&handle.mutex);
}


void ewol::simpleMsg::Clear(simpleMsg_ts& handle)
{
	if (false == handle.isInit) {
		return;
	}
	pthread_mutex_lock(&handle.mutex);
	if (handle.messageValue !=0) {
		struct timespec timeout;
		timeout.tv_sec = 0;
		timeout.tv_nsec = 0;
		pthread_cond_timedwait(&handle.condition, &handle.mutex, &timeout);
		
	}
	pthread_mutex_unlock(&handle.mutex);
}



