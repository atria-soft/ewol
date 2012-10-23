/**
 *******************************************************************************
 * @file ewol/widget/Entry.h
 * @brief ewol entry widget system (header)
 * @author Edouard DUPIN
 * @date 07/11/2011
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

#ifndef __EWOL_ENTRY_H__
#define __EWOL_ENTRY_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/oObject/OObject.h>
#include <ewol/widget/Widget.h>

extern const char * const ewolEventEntryClick;
extern const char * const ewolEventEntryEnter;
extern const char * const ewolEventEntryModify; // return in the data the new string inside it ...

namespace ewol {
	class Entry : public ewol::Widget
	{
		public:
			Entry(void);
			Entry(etk::UString newData);
			virtual ~Entry(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolEntry"; };
			void Init(void);
			virtual bool   CalculateMinSize(void);
			void           SetValue(etk::UString newData);
			etk::UString   GetValue(void);
			void           SetWidth(int32_t width)
			{
				m_userSize = width;
			}
		private:
			ewol::OObject2DTextColored m_oObjectText;
			ewol::OObject2DColored     m_oObjectDecoration;
			etk::UString   m_data;
			draw::Color    m_textColorFg;  //!< Text color
			draw::Color    m_textColorBg;  //!< Background color
			int32_t        m_userSize;
			int32_t        m_displayStartPosition;
			int32_t        m_displayCursorPos;
			int32_t        m_borderSize;
			int32_t        m_paddingSize;
			void           UpdateTextPosition(void);
			bool           m_displayCursor;
		public:
			virtual void OnRegenerateDisplay(void);
			virtual void OnDraw(DrawProperty& displayProp);
		public:
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, Vector2D<float> pos);
			/**
			 * @brief Event on the keybord (if no shortcut has been detected before).
			 * @param[in] type of the event (ewol::EVENT_KB_TYPE_DOWN or ewol::EVENT_KB_TYPE_UP)
			 * @param[in] unicodeValue key pressed by the user
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKb(eventKbType_te typeEvent, uniChar_t unicodeData);
			/**
			 * @brief Event on the keyboard that is not a printable key (if no shortcut has been detected before).
			 * @return true if the event has been used
			 * @return false if the event has not been used
			 */
			virtual bool OnEventKbMove(eventKbType_te typeEvent, eventKbMoveType_te moveTypeEvent);
			/**
			 * @brief Receive a message from an other EObject with a specific eventId and data
			 * @param[in] CallerObject Pointer on the EObject that information came from
			 * @param[in] eventId Message registered by this class
			 * @param[in] data Data registered by this class
			 * @return ---
			 */
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			/**
			 * @brief Event on a past event ==> this event is asynchronous due to all system does not support direct getting datas
			 * @note : need to have focus ...
			 * @param[in] mode Mode of data requested
			 * @return ---
			 */
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te clipboardID);
		protected:
			/**
			 * @brief Change the cursor position with the curent position requested on the display
			 * @param[in] pos Absolute position of the event
			 * @note The display is automaticly requested when change apear.
			 * @return ---
			 */
			virtual void UpdateCursorPosition(Vector2D<float>& pos);
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
	
};

#endif
