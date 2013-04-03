/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/key.h>
#include <ewol/config.h>
#include <ewol/commandLine.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/renderer/os/gui.h>

#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/os/eSystem.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <sys/times.h>
#include <mach/clock.h>
#include <mach/mach.h>

int64_t guiInterface::GetTime(void)
{
	struct timespec now;
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	now.tv_sec = mts.tv_sec;
	now.tv_nsec = mts.tv_nsec;
	//EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
	return (int64_t)((int64_t)now.tv_sec*(int64_t)1000000 + (int64_t)now.tv_nsec/(int64_t)1000);
}

#undef __class__
#define __class__	"guiInterface"



static ewol::SpecialKey guiKeyBoardMode;






/**
 * @brief Set the new title of the windows
 * @param title New desired title
 * @return ---
 */
void guiInterface::SetTitle(etk::UString& title)
{
	EWOL_INFO("X11: Set Title (START)");
	EWOL_INFO("X11: Set Title (END)");
}

void guiInterface::SetIcon(etk::UString inputFile)
{
	EWOL_TODO("plop");	
}


void RemoveDecoration(void)
{
	EWOL_TODO("X11:RemoveDecoration");
}

void AddDecoration(void)
{
	EWOL_TODO("X11:AddDecoration");
}



// -------------------------------------------------------------------------
//         ClipBoard AREA :
// -------------------------------------------------------------------------


void guiInterface::ClipBoardGet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	
}


void guiInterface::ClipBoardSet(ewol::clipBoard::clipboardListe_te clipboardID)
{
	
}



#undef __class__
#define __class__ "guiInterface"


void guiInterface::Stop(void)
{
	EWOL_INFO("X11-API: Stop");
}

void guiInterface::KeyboardShow(void)
{
	// nothing to do : No keyboard on computer ...
}


void guiInterface::KeyboardHide(void)
{
	// nothing to do : No keyboard on computer ...
}


void guiInterface::ChangeSize(ivec2 size)
{
	EWOL_INFO("X11-API: ChangeSize=" << size);
}


void guiInterface::ChangePos(ivec2 pos)
{
	EWOL_INFO("X11-API: ChangePos=" << pos);
}


void guiInterface::GetAbsPos(ivec2& pos)
{
	EWOL_INFO("X11-API: GetAbsPos");
}

// select the current cursor to display :
static ewol::cursorDisplay_te l_currentCursor = ewol::cursorArrow;

void guiInterface::SetCursor(ewol::cursorDisplay_te newCursor)
{
	if (newCursor != l_currentCursor) {
		EWOL_DEBUG("X11-API: Set New Cursor : " << newCursor);
		// set the new one :
		l_currentCursor = newCursor;
	}
}

void guiInterface::GrabPointerEvents(bool isGrabbed, vec2 forcedPosition)
{
	
}

#include <ewol/renderer/os/gui.MacOs.Interface.h>

/**
 * @brief Main of the program
 * @param std IO
 * @return std IO
 */
int guiInterface::main(int argc, const char *argv[])
{
	//start the basic thread : 
	eSystem::Init();
	// Run ...
	int ret = mm_main(argc, argv);
	EWOL_INFO("Return main value=" << ret);
	// close X11 :
	guiInterface::Stop();
	// uninit ALL :
	eSystem::UnInit();
	return 0;
}


void guiInterface::ForceOrientation(ewol::orientation_te orientation)
{
	// nothing to do ...
}






