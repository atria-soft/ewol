/**
 *******************************************************************************
 * @file guiAndroid.cpp
 * @brief Gui abstraction layer (Sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
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



#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>

#include <ewol/Debug.h>
#include <etk/UString.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/os/gui.h>
#include <ewol/os/eSystem.h>
#include <ewol/ewol.h>

#include <ewol/openGl.h>
#include <ewol/texture/Texture.h>

#undef __class__
#define __class__ "AndroidJNI"

int64_t guiInterface::GetTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
    return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_usec);
}


/**
 * @brief Set the new title of the windows
 * @param title New desired title
 * @return ---
 */
void guiInterface::SetTitle(etk::UString& title)
{
	// can not set the title in Android ...
}


// -------------------------------------------------------------------------
//         ClipBoard AREA :
// -------------------------------------------------------------------------

bool l_clipBoardOwnerStd = false;
/**
 * @brief Inform the Gui that we want to have a copy of the clipboard
 * @param ID of the clipboard (STD/SELECTION) only apear here
 * @return ---
 */
void guiInterface::ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	// this is to force the local system to think we have the buffer
	// TODO : Remove this 2 Line when code will be writen
	l_clipBoardOwnerStd = true;
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			// NOTE : Windows does not support the middle button the we do it internaly
			// just transmit an event , we have the data in the system
			eSystem::ClipBoardArrive(clipboardID);
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			if (false == l_clipBoardOwnerStd) {
				// Generate a request TO the OS
				// TODO : Send the message to the OS "We disire to receive the copy buffer ...
			} else {
				// just transmit an event , we have the data in the system
				eSystem::ClipBoardArrive(clipboardID);
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}

/**
 * @brief Inform the Gui that we are the new owner of the clipboard
 * @param ID of the clipboard (STD/SELECTION) only apear here
 * @return ---
 */
void guiInterface::ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	switch (clipboardID)
	{
		case ewol::clipBoard::CLIPBOARD_SELECTION:
			// NOTE : nothing to do : Windows deas ot supported Middle button
			break;
		case ewol::clipBoard::CLIPBOARD_STD:
			// Request the clipBoard :
			if (false == l_clipBoardOwnerStd) {
				// TODO : Inform the OS that we have the current buffer of copy ...
				l_clipBoardOwnerStd = true;
			}
			break;
		default:
			EWOL_ERROR("Request an unknow ClipBoard ...");
			break;
	}
}


#undef __class__
#define __class__ "guiInterface"

void guiInterface::Stop(void)
{
	// TODO : send a message to the android system to stop ...
}

// java system to send message : 
void SendSystemMessage(const char * dataString);
void SendJava_KeyboardShow(bool showIt);

void guiInterface::KeyboardShow(void)
{
	// send a message at the java :
	SendJava_KeyboardShow(true);
}

void guiInterface::KeyboardHide(void)
{
	// send a message at the java :
	SendJava_KeyboardShow(false);
}

void guiInterface::ChangeSize(Vector2D<int32_t> size)
{
	// The size can not be change on android platform
}

void guiInterface::ChangePos(Vector2D<int32_t> size)
{
	// The position can not be change on Android platform
}

void guiInterface::GetAbsPos(Vector2D<int32_t>& size)
{
	size.x = 0;
	size.y = 0;
}

