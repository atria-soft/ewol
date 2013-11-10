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

namespace ewol {
	namespace keyEvent {
		/**
		 * @brief type of input : Note that the keyboard is not prevent due to the fact that data is too different
		 */
		enum type {
			typeUnknow = 0, //!< Unknow input Type
			typeMouse,      //!< Mouse type
			typeFinger,     //!< Finger type
			typeStylet,     //!< Stylet type
			typeCount       //!< number of types
		};
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout& _os, const enum ewol::keyEvent::type _obj);
		/**
		 * @brief Keybord event or joyestick event
		 */
		enum status {
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
		};
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout& _os, const enum ewol::keyEvent::status _obj);
		/**
		 * @brief Keybord event or joyestick event
		 */
		enum keyboard {
			keyboardUnknow = 0,    //!< Unknown keyboard key
			keyboardChar,          //!< Char input is arrived ...
			keyboardLeft,          //!< Left key <--
			keyboardRight,         //!< Right key -->
			keyboardUp,            //!< Up key ^
			keyboardDown,          //!< Down key \/
			keyboardPageUp,        //!< Page Up key
			keyboardPageDown,      //!< page down key
			keyboardStart,         //!< Start key
			keyboardEnd,           //!< End key
			keyboardPrint,         //!< print screen key.
			keyboardStopDefil,     //!< Stop display key.
			keyboardWait,          //!< Wait key.
			keyboardInsert,        //!< insert key.
			keyboardF1,            //!< F1 key.
			keyboardF2,            //!< F2 key.
			keyboardF3,            //!< F3 key.
			keyboardF4,            //!< F4 key.
			keyboardF5,            //!< F5 key.
			keyboardF6,            //!< F6 key.
			keyboardF7,            //!< F7 key.
			keyboardF8,            //!< F8 key.
			keyboardF9,            //!< F9 key.
			keyboardF10,           //!< F10 key.
			keyboardF11,           //!< F11 key.
			keyboardF12,           //!< F12 key.
			keyboardCapLock,       //!< Capital Letter Lock key.
			keyboardShiftLeft,     //!< Shift left key.
			keyboardShiftRight,    //!< Shift right key.
			keyboardCtrlLeft,      //!< Control left key.
			keyboardCtrlRight,     //!< Control right key.
			keyboardMetaLeft,      //!< Meta left key (apple key or windows key).
			keyboardMetaRight,     //!< Meta right key (apple key or windows key).
			keyboardAlt,           //!< Alt key.
			keyboardAltGr,         //!< Alt ground key.
			keyboardContextMenu,   //!< Contextual menu key.
			keyboardNumLock,       //!< Numerical Lock key.
			keyboardCount          //!< number of posible key
		};
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout& _os, const enum ewol::keyEvent::keyboard _obj);
		
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
					unsigned numLock   : 1;
					unsigned insert    : 1;
				};
			};
		public:
			/**
			 * @brief Main constructor
			 */
			SpecialKey(void);
			/**
			 * @brief get the current CapLock Status
			 * @return The status value
			 */
			bool isSetCapsLock(void) const;
			/**
			 * @brief get the current Shift key status
			 * @return The status value
			 */
			bool isSetShift(void) const;
			/**
			 * @brief get the Current Control key status
			 * @return The status value
			 */
			bool isSetCtrl(void) const;
			/**
			 * @brief get the current Meta key status (also named windows or apple key)
			 * @return The status value
			 */
			bool isSetMeta(void) const;
			/**
			 * @brief get the current Alt key status
			 * @return The status value
			 */
			bool isSetAlt(void) const;
			/**
			 * @brief get the current Alt-Gr key status
			 * @return The status value
			 */
			bool isSetAltGr(void) const;
			/**
			 * @brief get the current Ver-num key status
			 * @return The status value
			 */
			bool isSetNumLock(void) const;
			/**
			 * @brief get the current Intert key status
			 * @return The status value
			 */
			bool isSetInsert(void) const;
	};
	/**
	 * @brief Debug operator To display the curent element in a Human redeable information
	 */
	etk::CCout& operator <<(etk::CCout& _os, const ewol::SpecialKey _obj);
};



#endif


