/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_ENTRY_H__
#define __EWOL_ENTRY_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Widget.h>
#include <draw/Color.h>

extern const char * const ewolEventEntryClick;
extern const char * const ewolEventEntryEnter;
extern const char * const ewolEventEntryModify; // return in the data the new string inside it ...

namespace widget {
	/**
	 * @brief Entry box display :
	 *
	 * @code
	 *  ----------------------------------------------
	 *  | Editable Text                              |
	 *  ----------------------------------------------
	 * @endcode
	 */
	class Entry : public ewol::Widget
	{
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Shaper m_shaper;
			ewol::Text   m_oObjectText;               //!< text display
			etk::UString m_data;                      //!< sting that must be displayed
			draw::Color  m_textColorFg;               //!< Text color
			draw::Color  m_textColorBg;               //!< Background color
			int32_t      m_userSize;                  //!< Display size requested by the user
			int32_t      m_displayStartPosition;      //!< ofset in pixel of the display of the UString
			bool         m_displayCursor;             //!< Cursor mus be display only when the widget has the focus
			int32_t      m_displayCursorPos;          //!< Cursor position in number of Char
			int32_t      m_displayCursorPosSelection; //!< Selection position end (can be befor or after cursor and == m_displayCursorPos chan no selection availlable
		public:
			/**
			 * @brief Contuctor
			 * @param[in] newData The USting that might be set in the Entry box (no event generation!!)
			 */
			Entry(etk::UString newData = "");
			/**
			 * @brief Destuctor
			 */
			virtual ~Entry(void);
			void SetValue(etk::UString newData);
			etk::UString GetValue(void);
			void SetWidth(int32_t width)
			{
				m_userSize = width;
			}
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te typeEvent, const vec2& pos);
			virtual bool OnEventKb(ewol::keyEvent::status_te typeEvent, uniChar_t unicodeData);
			virtual bool OnEventKbMove(ewol::keyEvent::status_te typeEvent, ewol::keyEvent::keyboard_te moveTypeEvent);
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, const etk::UString& data);
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
			virtual const char * const GetObjectType(void) { return "EwolEntry"; };
			virtual void CalculateMinMaxSize(void);
		protected:
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
			/**
			 * @brief Change the cursor position with the curent position requested on the display
			 * @param[in] pos Absolute position of the event
			 * @note The display is automaticly requested when change apear.
			 * @return ---
			 */
			virtual void UpdateCursorPosition(const vec2& pos, bool Selection=false);
			/**
			 * @brief Update the display position start ==> depending of the position of the Cursor and the size of the Data inside
			 * @param ---
			 * @return ---
			 * @change m_displayStartPosition <== updated
			 */
			virtual void UpdateTextPosition(void);
			/**
			 * @brief Copy the selected data on the specify clipboard
			 * @param[in] clipboardID Selected clipboard
			 * @return ---
			 */
			virtual void CopySelectionToClipBoard(ewol::clipBoard::clipboardListe_te clipboardID);
			/**
			 * @brief Remove the selected area
			 * @note This request a regeneration of the display
			 * @return ---
			 */
			virtual void RemoveSelected(void);
			// Derived function
			virtual void OnGetFocus(void);
			// Derived function
			virtual void OnLostFocus(void);
			// change the current shaper display :
			void ChangeStatusIn(int32_t newStatusId);
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
	
};

#endif
