/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_KEY_H__
#define __EWOL_KEY_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol
{
	namespace keyEvent
	{
		/**
		 * @brief type of input : Note that the keyboard is not prevent due to the fact that data is too different
		 */
		typedef enum {
			typeUnknow = 0,
			typeMouse,
			typeFinger,
			typeStylet,
			typeCount
		} type_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout &os, const ewol::keyEvent::type_te obj);
		/**
		 * @brief Keybord event or joyestick event
		 */
		typedef enum {
			statusUnknow = 0,
			statusDown, // availlable on Keyboard too
			statusMove,
			statusSingle,
			statusDouble,
			statusTriple,
			statusQuad,
			statusQuinte,
			statusUp, // availlable on Keyboard too
			statusEnter,
			statusLeave,
			statusAbort, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has lost the events)
			statusTransfert, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has receive the transfert of the event)
			statusCount, // number max of imput possible
		} status_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout &os, const ewol::keyEvent::status_te obj);
		/**
		 * @brief Keybord event or joyestick event
		 */
		typedef enum {
			keyboardUnknow = 0,
			keyboardLeft,
			keyboardRight,
			keyboardUp,
			keyboardDown,
			keyboardPageUp,
			keyboardPageDown,
			keyboardStart,
			keyboardEnd,
			keyboardCenter,
			keyboardStopDefil,
			keyboardWait,
			keyboardInsert,
			keyboardF1,
			keyboardF2,
			keyboardF3,
			keyboardF4,
			keyboardF5,
			keyboardF6,
			keyboardF7,
			keyboardF8,
			keyboardF9,
			keyboardF10,
			keyboardF11,
			keyboardF12,
			keyboardCapLock,
			keyboardShiftLeft,
			keyboardShiftRight,
			keyboardCtrlLeft,
			keyboardCtrlRight,
			keyboardMetaLeft,
			keyboardMetaRight,
			keyboardAlt,
			keyboardAltGr,
			keyboardContextMenu,
			keyboardVerNum,
			keyboardCount
		} keyboard_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout &os, const ewol::keyEvent::keyboard_te obj);
		
	};
	
	class SpecialKey {
		public:
			union {
				uint32_t value;
				struct {
					unsigned capLock   : 1;
					unsigned shift     : 1;
					unsigned ctrl      : 1;
					unsigned meta      : 1;
					unsigned alt       : 1;
					unsigned altGr     : 1;
					unsigned verNum    : 1;
					unsigned insert    : 1;
				};
			};
		SpecialKey(void) :
			value(0)
		{
			
		}
		bool IsSetCapsLock(void)
		{
			return capLock;
		}
		
		bool IsSetShift(void)
		{
			return shift;
		}
		
		bool IsSetCtrl(void)
		{
			return ctrl;
		}
		
		bool IsSetMeta(void)
		{
			return meta;
		}
		
		bool IsSetAlt(void)
		{
			return alt;
		}
		
		bool IsSetAltGr(void)
		{
			return altGr;
		}
		
		bool IsSetVerNum(void)
		{
			return verNum;
		}
		bool IsSetInsert(void)
		{
			return insert;
		}
	};
	
	SpecialKey& GetCurrentSpecialKeyStatus(void);
};



#endif


