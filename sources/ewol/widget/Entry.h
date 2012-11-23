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
#include <ewol/oObject/OObject.h>
#include <ewol/widget/Widget.h>
#include <draw/Color.h>

extern const char * const ewolEventEntryClick;
extern const char * const ewolEventEntryEnter;
extern const char * const ewolEventEntryModify; // return in the data the new string inside it ...

namespace ewol {
	/**
	 * @brief Entry box display :
	 *
	 * @code
	 *  ----------------------------------------------
	 *  | Mon Texte Modi|fiable                      |
	 *  ----------------------------------------------
	 * @endcode
	 */
	class Entry : public ewol::Widget
	{
		private:
			ewol::Program* m_GLprogram;
			int32_t        m_GLPosition;
			int32_t        m_GLMatrix;
			int32_t        m_GLsizeBorder;
			int32_t        m_GLsizePadding;
			int32_t        m_GLsize;
			float          m_pos[4];
			int32_t        m_GLposText;
			int32_t        m_GLstate;
			etk::Vector<etk::Vector2D<float> > m_coord;       //!< internal coord of the object
			draw::Colorf                  m_color[3];
			void SetPoint(float x, float y);
			void Rectangle(float x, float y, float w, float h);
		private:
			ewol::OObject2DTextColored m_oObjectText;               //!< text display
			ewol::OObject2DColored     m_oObjectDecoration;         //!< background display
			etk::UString               m_data;                      //!< sting that must be displayed
			draw::Color                m_textColorFg;               //!< Text color
			draw::Color                m_textColorBg;               //!< Background color
			int32_t                    m_userSize;                  //!< Display size requested by the user
			int32_t                    m_displayStartPosition;      //!< ofset in pixel of the display of the UString
			int32_t                    m_borderSize;                //!< Border size
			int32_t                    m_paddingSize;               //!< space between border and the text and the border base and the border widget
			bool                       m_displayCursor;             //!< Cursor mus be display only when the widget has the focus
			int32_t                    m_displayCursorPos;          //!< Cursor position in number of Char
			int32_t                    m_displayCursorPosSelection; //!< Selection position end (can be befor or after cursor and == m_displayCursorPos chan no selection availlable
		public:
			/**
			 * @brief Contuctor
			 */
			Entry(void);
			/**
			 * @brief Contuctor
			 * @param[in] newData The USting that might be set in the Entry box (no event generation!!)
			 */
			Entry(etk::UString newData);
			/**
			 * @brief Destuctor
			 */
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
		public:
			/**
			 * @brief Event generated when a redraw is needed
			 * @param ---
			 * @return ---
			 */
			virtual void OnRegenerateDisplay(void);
			/**
			 * @brief Event on an input of this Widget
			 * @param[in] type Type of the input (ewol::INPUT_TYPE_MOUSE/ewol::INPUT_TYPE_FINGER ...)
			 * @param[in] IdInput Id of the current Input (PC : left=1, right=2, middle=3, none=0 / Tactil : first finger=1 , second=2 (only on this widget, no knowledge at ouside finger))
			 * @param[in] typeEvent ewol type of event like EVENT_INPUT_TYPE_DOWN/EVENT_INPUT_TYPE_MOVE/EVENT_INPUT_TYPE_UP/EVENT_INPUT_TYPE_SINGLE/EVENT_INPUT_TYPE_DOUBLE/...
			 * @param[in] pos Absolute position of the event
			 * @return true the event is used
			 * @return false the event is not used
			 */
			virtual bool OnEventInput(ewol::inputType_te type, int32_t IdInput, eventInputType_te typeEvent, etk::Vector2D<float> pos);
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
			 * @brief Common widget drawing function (called by the drawing thread [Android, X11, ...])
			 * @param[in] displayProp properties of the current display
			 * @return ---
			 */
			virtual void OnDraw(DrawProperty& displayProp);
			/**
			 * @brief Change the cursor position with the curent position requested on the display
			 * @param[in] pos Absolute position of the event
			 * @note The display is automaticly requested when change apear.
			 * @return ---
			 */
			virtual void UpdateCursorPosition(etk::Vector2D<float>& pos, bool Selection=false);
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
			/**
			 * @brief Event of the focus has been grep by the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void OnGetFocus(void);
			/**
			 * @brief Event of the focus has been lost by the current widget
			 * @param ---
			 * @return ---
			 */
			virtual void OnLostFocus(void);
	};
	
};

#endif
