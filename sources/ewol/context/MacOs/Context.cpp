/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/debug.h>
#include <ewol/ewol.h>
#include <ewol/key/key.h>
#include <ewol/context/commandLine.h>
#include <etk/UString.h>
#include <etk/unicode.h>
#include <etk/os/FSNode.h>
#include <ewol/widget/Manager.h>

#include <ewol/resource/Manager.h>
#include <ewol/context/Context.h>

#include <ewol/context/MacOs/Interface.h>
#include <ewol/context/MacOs/Context.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <sys/times.h>
#include <mach/clock.h>
#include <mach/mach.h>


int64_t ewol::getTime(void) {
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



class MacOSInterface : public ewol::Context {
	private:
	ewol::key::Special m_guiKeyBoardMode;
	public:
		MacOSInterface(int32_t _argc, const char* _argv[]) :
		  ewol::Context(_argc, _argv) {
			mm_main(_argc, _argv);
		}
		
		int32_t Run(void) {
			return mm_run();
		}
	public:
		//interface MacOS :
		
		bool MAC_Draw(bool _displayEveryTime) {
			return OS_Draw(_displayEveryTime);
		}
		void MAC_Resize(float _x, float _y) {
			OS_Resize(vec2(_x,_y));
		}
		void MAC_SetMouseState(int32_t _id, bool _isDown, float _x, float _y) {
			OS_SetMouseState(_id, _isDown, vec2(_x, _y));
		}
		void MAC_SetMouseMotion(int32_t _id, float _x, float _y) {
			OS_SetMouseMotion(_id, vec2(_x, _y));
		}
		void MAC_SetKeyboard(ewol::key::Special _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey) {
			if (_unichar == etk::UChar::Delete) {
				_unichar = etk::UChar::Suppress;
			} else if (_unichar == etk::UChar::Suppress) {
				_unichar = etk::UChar::Delete;
			}
			if (_unichar == etk::UChar::CarrierReturn) {
				_unichar = etk::UChar::Return;
			}
			//EWOL_DEBUG("key: " << _unichar << " up=" << !_isDown);
			if (_unichar <= 4) {
				enum ewol::key::keyboard move;
				switch(_unichar) {
					case 0:
						move = ewol::key::keyboardUp;
						break;
					case 1:
						move = ewol::key::keyboardDown;
						break;
					case 2:
						move = ewol::key::keyboardLeft;
						break;
					case 3:
						move = ewol::key::keyboardRight;
						break;
				}
				OS_SetKeyboardMove(_keyboardMode, move, !_isDown, _isAReapeateKey);
			} else {
				OS_SetKeyboard(_keyboardMode, _unichar, !_isDown, _isAReapeateKey);
			}
		}
	void MAC_SetKeyboardMove(ewol::key::Special& _special,
								enum ewol::key::keyboard _move,
								 bool _isDown) {
			OS_SetKeyboardMove(_special, _move, _isDown);
		}
};


MacOSInterface* interface = NULL;



bool MacOs::draw(bool _displayEveryTime) {
	if (interface == NULL) {
		return false;
	}
	return interface->MAC_Draw(_displayEveryTime);
}

void MacOs::resize(float _x, float _y) {
	if (interface == NULL) {
		return;
	}
	interface->MAC_Resize(_x, _y);
}


void MacOs::setMouseState(int32_t _id, bool _isDown, float _x, float _y) {
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetMouseState(_id, _isDown, _x, _y);
}

void MacOs::setMouseMotion(int32_t _id, float _x, float _y) {
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetMouseMotion(_id, _x, _y);
}

void MacOs::setKeyboard(ewol::key::Special _keyboardMode, int32_t _unichar, bool _isDown, bool _isAReapeateKey) {
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetKeyboard(_keyboardMode, _unichar, _isDown, _isAReapeateKey);
}

void MacOs::setKeyboardMove(ewol::key::Special& _keyboardMode, enum ewol::key::keyboard _move, bool _isDown) {
	if (interface == NULL) {
		return;
	}
	interface->MAC_SetKeyboardMove(_keyboardMode, _move, _isDown);
}


/**
 * @brief Main of the program
 * @param std IO
 * @return std IO
 */
int ewol::run(int _argc, const char *_argv[]) {
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







