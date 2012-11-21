/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_KEY_H__
#define __EWOL_KEY_H__

#include <etk/Types.h>
#include <etk/Stream.h>

namespace ewol
{
	namespace keyEvent
	{
		// event from all the type of input :
		typedef enum {
			None,
			MouseDown,
			MouseMove,
			MouseSingle,
			MouseDouble,
			MouseTriple,
			MouseQuad,
			MouseQuinte,
			MouseUp,
			MouseEnter,
			MouseLeave,
			MouseAbort, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has lost the events)
			MouseTransfer, // Appeare when an event is tranfert betwwen widgets (the widget which receive this has receive the transfert of the event)
			KeyboardLeft,
			KeyboardRight,
			KeyboardUp,
			KeyboardDown,
			KeyboardPageUp,
			KeyboardPageDown,
			KeyboardStart,
			KeyboardEnd,
			KeyboardCenter,
			KeyboardStopDefil,
			KeyboardWait,
			KeyboardInsert,
			KeyboardF1,
			KeyboardF2,
			KeyboardF3,
			KeyboardF4,
			KeyboardF5,
			KeyboardF6,
			KeyboardF7,
			KeyboardF8,
			KeyboardF9,
			KeyboardF10,
			KeyboardF11,
			KeyboardF12,
			KeyboardCapLock,
			KeyboardShiftLeft,
			KeyboardShiftRight,
			KeyboardCtrlLeft,
			KeyboardCtrlRight,
			KeyboardMetaLeft,
			KeyboardMetaRight,
			KeyboardAlt,
			KeyboardAltGr,
			KeyboardContextMenu,
			KeyboardVerNum,
		} event_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout &os, const ewol::keyEvent::event_te obj);
		
		// type of input : Notye that the keyboard is not prevent due to the fact that data is too different
		typedef enum {
			Unknow,
			Mouse,
			Finger,
			Stylet,
		} input_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout &os, const ewol::keyEvent::input_te obj);
	};
};



#endif


