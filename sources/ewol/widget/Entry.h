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
#include <etk/RegExp.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>

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
			// Event list of properties
			static const char * const eventClick;
			static const char * const eventEnter;
			static const char * const eventModify; // return in the data the new string inside it ...
			// Config list of properties
			static const char* const configMaxChar;
			static const char* const configRegExp;
			static const char* const configColorFg;
			static const char* const configColorBg;
			static const char* const configEmptyMessage;
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Shaper m_shaper;
			ewol::Text m_oObjectText; //!< text display m_text
		public:
			/**
			 * @brief Contuctor
			 * @param[in] _newData The USting that might be set in the Entry box (no event generation!!)
			 */
			Entry(etk::UString _newData = "");
			/**
			 * @brief Destuctor
			 */
			virtual ~Entry(void);
		
		private:
			etk::UString m_data; //!< sting that must be displayed
		protected:
			/**
			 * @brief internal check the value with RegExp checking
			 * @param[in] _newData The new string to display
			 */
			void SetInternalValue(const etk::UString& _newData);
		public:
			/**
			 * @brief set a new value on the entry.
			 * @param[in] _newData the new string to display.
			 */
			void SetValue(const etk::UString& _newData);
			/**
			 * @brief Get the current value in the entry
			 * @return The current display value
			 */
			etk::UString GetValue(void) const { return m_data; };
		
		private:
			int32_t m_maxCharacter; //!< number max of xharacter in the list
		public:
			/**
			 * @brief Limit the number of Unicode character in the entry
			 * @param[in] _nbMax Number of max character set in the List (0x7FFFFFFF for no limit)
			 */
			void SetMaxChar(int32_t _nbMax);
			/**
			 * @brief Limit the number of Unicode character in the entry
			 * @return Number of max character set in the List.
			 */
			int32_t GetMaxChar(void) const { return m_maxCharacter; };
		
		private:
			etk::RegExp<etk::UString> m_regExp; //!< regular expression to limit the input of an entry
		public:
			/**
			 * @brief Limit the input entry at a regular expression... (by default it is "*")
			 * @param _expression New regular expression
			 */
			void SetRegExp(const etk::UString& _expression);
			/**
			 * @brief Get the regualar expression limitation
			 * @param The regExp string
			 */
			const etk::UString& GetRegExp(void) const { return m_regExp.GetRegExp(); };
		
		private:
			bool m_needUpdateTextPos; //!< text position can have change
			int32_t m_displayStartPosition; //!< ofset in pixel of the display of the UString
			bool m_displayCursor; //!< Cursor must be display only when the widget has the focus
			int32_t m_displayCursorPos; //!< Cursor position in number of Char
			int32_t m_displayCursorPosSelection; //!< Selection position end (can be befor or after cursor and == m_displayCursorPos chan no selection availlable
		protected:
			/**
			 * @brief informe the system thet the text change and the start position change
			 */
			virtual void MarkToUpdateTextPosition(void);
			/**
			 * @brief Update the display position start ==> depending of the position of the Cursor and the size of the Data inside
			 * @change m_displayStartPosition <== updated
			 */
			virtual void UpdateTextPosition(void);
			/**
			 * @brief Change the cursor position with the curent position requested on the display
			 * @param[in] _pos Absolute position of the event
			 * @note The display is automaticly requested when change apear.
			 */
			virtual void UpdateCursorPosition(const vec2& _pos, bool _Selection=false);
		
		public:
			/**
			 * @brief Copy the selected data on the specify clipboard
			 * @param[in] _clipboardID Selected clipboard
			 */
			virtual void CopySelectionToClipBoard(ewol::clipBoard::clipboardListe_te _clipboardID);
			/**
			 * @brief Remove the selected area
			 * @note This request a regeneration of the display
			 */
			virtual void RemoveSelected(void);
		
		private:
			etk::Color<> m_textColorFg; //!< Text color.
		public:
			/**
			 * @brief Set text color.
			 * @param _color Color that is selected.
			 */
			void SetColorText(const etk::Color<>& _color);
			/**
			 * @brief Get the color for the text.
			 * @return The color requested.
			 */
			const etk::Color<>& GetColorText(void) const { return m_textColorFg; };
		
		private:
			etk::Color<> m_textColorBg; //!< Background color.
		public:
			/**
			 * @brief Set text backgroung color when selected.
			 * @param _color Color that is selected.
			 */
			void SetColorTextSelected(const etk::Color<>& _color);
			/**
			 * @brief Get the selected color for the text in selection mode.
			 * @return The color requested.
			 */
			const etk::Color<>& GetColorTextSelected(void) const { return m_textColorBg; };
		
		private:
			etk::UString m_textWhenNothing; //!< Text to display when nothing in in the entry (decorated text...)
		public:
			/**
			 * @brief Set The text displayed when nothing is in the entry.
			 * @param _text Text to display when the entry box is empty (this text can be decorated).
			 */
			void SetEmptyText(const etk::UString& _text);
			/**
			 * @brief Get The text displayed when nothing is in the entry.
			 * @return Text display when nothing
			 */
			const etk::UString& GetEmptyText(void) const { return m_textWhenNothing; };
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool OnEventEntry(const ewol::EventEntry& _event);
			virtual void OnReceiveMessage(const ewol::EMessage& _msg);
			virtual void OnEventClipboard(ewol::clipBoard::clipboardListe_te _clipboardID);
			virtual const char * const GetObjectType(void) { return "EwolEntry"; };
			virtual void CalculateMinMaxSize(void);
		protected: // Derived function
			virtual void OnDraw(void);
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
			virtual void ChangeStatusIn(int32_t _newStatusId);
			virtual void PeriodicCall(const ewol::EventTime& _event);
			virtual bool OnSetConfig(const ewol::EConfig& _conf);
			virtual bool OnGetConfig(const char* _config, etk::UString& _result) const;
	};
	
};

#endif
