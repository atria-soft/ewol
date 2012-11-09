/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CLIPBOARD_H__
#define __EWOL_CLIPBOARD_H__

#include <ewol/Debug.h>
#include <etk/UString.h>


namespace ewol {
	namespace clipBoard
	{
		typedef enum {
			CLIPBOARD_0,         //!< Internal clipboard 0
			CLIPBOARD_1,         //!< Internal clipboard 1
			CLIPBOARD_2,         //!< Internal clipboard 2
			CLIPBOARD_3,         //!< Internal clipboard 3
			CLIPBOARD_4,         //!< Internal clipboard 4
			CLIPBOARD_5,         //!< Internal clipboard 5
			CLIPBOARD_6,         //!< Internal clipboard 6
			CLIPBOARD_7,         //!< Internal clipboard 7
			CLIPBOARD_8,         //!< Internal clipboard 8
			CLIPBOARD_9,         //!< Internal clipboard 9
			CLIPBOARD_STD,       //!< External clipboard represent the Copy/Cut/Past buffer
			CLIPBOARD_SELECTION, //!< External or internal clipboard depending on the OS, represent the middle button
			TOTAL_OF_CLICKBOARD, //!< Total number of clipboard
		} clipboardListe_te;
		/**
		 * @brief Initialize the clipboard system (done by ewol)
		 * @param ---
		 * @return ---
		 */
		void         Init(void);
		
		/**
		 * @brief Un-Initialize the clipboard system (done by ewol)
		 * @param ---
		 * @return ---
		 */
		void         UnInit(void);
		/**
		 * @brief Set the string data on a specific clipboard. The Gui system is notify that the clipboard "SELECTION" and "COPY" are change
		 * @param[in] clipboardID Select the specific ID of the clipboard
		 * @param[in] data The string that might be send to the clipboard
		 * @return ---
		 */
		void         Set(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data);
		/**
		 * @brief Call system to request the current clipboard.
		 * @note Due to some system that manage the clipboard request asynchronous (like X11) and ewol managing the system with only one thread,
		 *       we need the call the system to send us the buffer, this is really ambigous, but the widget (who has focus) receive the 
		 *       notification of the arrival of this buffer id
		 * @param[in] clipboardID the needed clipboard ID
		 * @return ---
		 */
		void         Request(ewol::clipBoard::clipboardListe_te clipboardID);
		/**
		 * @brief Set the ewol internal buffer (no notification at the GUI). This fuction might be use by the 
		 *        Gui abstraction to set the buffer we receive. The end user must not use it.
		 * @param[in] clipboardID selected clipboard ID
		 * @param[in] data new buffer data
		 * @return ---
		 */
		void         SetSystem(ewol::clipBoard::clipboardListe_te clipboardID, etk::UString &data);
		/**
		 * @brief Get the ewol internal buffer of the curent clipboard. The end user can use it when he receive the event in 
		 *        the widget : @ref OnEventClipboard ==> we can nothe this function is the only one which permit it.
		 * @note if we call this fuction withoutcallin @ref ewol::clipBoard::Request, we only get the previous clipboard
		 * @param[in] clipboardID selected clipboard ID
		 * @return the requested buffer
		 */
		etk::UString Get(ewol::clipBoard::clipboardListe_te clipboardID);
	};
};

#endif


