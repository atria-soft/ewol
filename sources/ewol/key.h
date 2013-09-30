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
			typeUnknow = 0, //!< Unknow input Type
			typeMouse,      //!< Mouse type
			typeFinger,     //!< Finger type
			typeStylet,     //!< Stylet type
			typeCount       //!< number of types
		} type_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout& _os, const ewol::keyEvent::type_te _obj);
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
		etk::CCout& operator <<(etk::CCout& _os, const ewol::keyEvent::status_te _obj);
		/**
		 * @brief Keybord event or joyestick event
		 */
		typedef enum {
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
			keyboardPrint,         //!< Print screen key.
			keyboardStopDefil,     //!< Stop display key.
			keyboardWait,          //!< Wait key.
			keyboardInsert,        //!< Insert key.
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
		} keyboard_te;
		/**
		 * @brief Debug operator To display the curent element in a Human redeable information
		 */
		etk::CCout& operator <<(etk::CCout& _os, const ewol::keyEvent::keyboard_te _obj);
		
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
			 * @brief Get the current CapLock Status
			 * @return The status value
			 */
			bool IsSetCapsLock(void) const;
			/**
			 * @brief Get the current Shift key status
			 * @return The status value
			 */
			bool IsSetShift(void) const;
			/**
			 * @brief Get the Current Control key status
			 * @return The status value
			 */
			bool IsSetCtrl(void) const;
			/**
			 * @brief Get the current Meta key status (also named windows or apple key)
			 * @return The status value
			 */
			bool IsSetMeta(void) const;
			/**
			 * @brief Get the current Alt key status
			 * @return The status value
			 */
			bool IsSetAlt(void) const;
			/**
			 * @brief Get the current Alt-Gr key status
			 * @return The status value
			 */
			bool IsSetAltGr(void) const;
			/**
			 * @brief Get the current Ver-num key status
			 * @return The status value
			 */
			bool IsSetNumLock(void) const;
			/**
			 * @brief Get the current Intert key status
			 * @return The status value
			 */
			bool IsSetInsert(void) const;
	};
	/**
	 * @brief Debug operator To display the curent element in a Human redeable information
	 */
	etk::CCout& operator <<(etk::CCout& _os, const ewol::SpecialKey _obj);
};



#endif


