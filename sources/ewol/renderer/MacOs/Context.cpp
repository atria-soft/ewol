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
#include <ewol/commandLine.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <etk/os/FSNode.h>
#include <ewol/widget/WidgetManager.h>

#include <ewol/resources/ResourceManager.h>
#include <ewol/renderer/eContext.h>

#include <ewol/renderer/MacOs/Interface.h>
#include <ewol/renderer/MacOs/Context.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <sys/times.h>
#include <mach/clock.h>
#include <mach/mach.h>


int64_t ewol::getTime(void)
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
#define __class__	"MacOSInterface"



class MacOSInterface : public ewol::eContext
{
	private:
		ewol::SpecialKey m_guiKeyBoardMode;
	public:
		MacOSInterface(int32_t _argc, const char* _argv[]) :
			ewol::eContext(_argc, _argv)
		{
			mm_main(_argc, _argv);
		}
		
		int32_t Run(void)
		{
    		return mm_run();
		}
	public:
		//interface MacOS :
		
		bool MAC_Draw(bool _displayEveryTime)
		{
			return OS_Draw(_displayEveryTime);
		}
		void MAC_Resize(float _x, float _y)
		{
			OS_Resize(vec2(_x,_y));
		}
		void MAC_SetMouseState(int32_t _id, bool _isDown, float _x, float _y)
		{
			OS_SetMouseState(_id, _isDown, vec2(_x, _y));
		}
		void MAC_SetMouseMotion(int32_t _id, float _x, float _y)
		{
			OS_SetMouseMotion(_id, vec2(_x, _y));
		}
		void MAC_SetKeyboard(ewol::SpecialKey _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey)
		{
			if (_unichar == '\r') {
				_unichar = '\n';
			}
			EWOL_DEBUG("key: " << _unichar << " up=" << !_isDown);
			OS_SetKeyboard(_keyboardMode, _unichar, !_isDown, _isAReapeateKey);
		}
};


MacOSInterface* interface = NULL;



bool MacOs::draw(bool _displayEveryTime)
{
	if (interface == NULL) {
		return false;
	}
	return interface->MAC_Draw(_displayEveryTime);
}

void MacOs::Resize(float _x, float _y)
{
	if (interface == NULL) {
		return;
	}
	interface->MAC_Resize(_x, _y);
}


void MacOs::setMouseState(int32_t _id, bool _isDown, float _x, float _y)
{
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetMouseState(_id, _isDown, _x, _y);
}

void MacOs::setMouseMotion(int32_t _id, float _x, float _y)
{
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetMouseMotion(_id, _x, _y);
}

void MacOs::setKeyboard(ewol::SpecialKey _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey)
{
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetKeyboard(_keyboardMode, _unichar, _isDown, _isAReapeateKey);
}



/**
 * @brief Main of the program
 * @param std IO
 * @return std IO
 */
int ewol::Run(int _argc, const char *_argv[])
{
	etk::setArgZero(_argv[0]);
	interface = new MacOSInterface(_argc, _argv);
	if (NULL == interface) {
		EWOL_CRITICAL("Can not create the X11 interface ... MEMORY allocation error");
		return -2;
	}
	
	int32_t retValue = interface->Run();
	delete(interface);
	interface = NULL;
	return retValue;
}







