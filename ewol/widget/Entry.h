/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_ENTRY_H__
#define __EWOL_ENTRY_H__

#include <etk/types.h>
#include <regex>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/compositing/Shaper.h>
#include <ewol/widget/Widget.h>
#include <etk/Color.h>
#include <ewol/widget/Manager.h>
#include <ewol/signal/Signal.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief Entry box display :
		 *
		 * ~~~~~~~~~~~~~~~~~~~~~~
		 * 	----------------------------------------------
		 * 	| Editable Text                              |
		 * 	----------------------------------------------
		 * ~~~~~~~~~~~~~~~~~~~~~~
		 */
		class Entry : public ewol::Widget {
			public:
				ewol::Signal<void> signalClick; //!< bang on click the entry box
				ewol::Signal<std::string> signalEnter; //!< Enter key is pressed
				ewol::Signal<std::string> signalModify; //!< data change
			private:
				ewol::parameter::Value<ewol::compositing::Shaper> m_shaper;
				int32_t m_colorIdTextFg; //!< color property of the text foreground
				int32_t m_colorIdTextBg; //!< color property of the text background
				int32_t m_colorIdCursor; //!< color property of the text cursor
				int32_t m_colorIdSelection; //!< color property of the text selection
				ewol::compositing::Text m_text; //!< text display m_text
			public:
				/**
				 * @brief Contuctor
				 * @param[in] _newData The USting that might be set in the Entry box (no event generation!!)
				 */
				Entry();
				void init(const std::string& _newData = "");
			public:
				DECLARE_WIDGET_FACTORY(Entry, "Entry");
				/**
				 * @brief Destuctor
				 */
				virtual ~Entry();
			private:
				ewol::parameter::Value<std::string> m_data; //!< sting that must be displayed
			protected:
				/**
				 * @brief internal check the value with RegExp checking
				 * @param[in] _newData The new string to display
				 */
				void setInternalValue(const std::string& _newData);
			public:
				/**
				 * @brief set a new value on the entry.
				 * @param[in] _newData the new string to display.
				 */
				void setValue(const std::string& _newData);
				/**
				 * @brief get the current value in the entry
				 * @return The current display value
				 */
				std::string getValue() const {
					return m_data;
				};
			private:
				ewol::parameter::Range<int32_t> m_maxCharacter; //!< number max of xharacter in the list
			public:
				/**
				 * @brief Limit the number of Unicode character in the entry
				 * @param[in] _nbMax Number of max character set in the List (0x7FFFFFFF for no limit)
				 */
				void setMaxChar(int32_t _nbMax) {
					m_maxCharacter.set(_nbMax);
				}
				/**
				 * @brief Limit the number of Unicode character in the entry
				 * @return Number of max character set in the List.
				 */
				int32_t getMaxChar() const {
					return m_maxCharacter;
				};
			private:
				ewol::parameter::Value<std::string> m_regexValue; //!< regular expression value
				std::regex m_regex; //!< regular expression to check content
			public:
				/**
				 * @brief Limit the input entry at a regular expression... (by default it is "*")
				 * @param _expression New regular expression
				 */
				void setRegex(const std::string& _expression) {
					m_regexValue.set(_expression);
				}
				/**
				 * @brief get the regualar expression limitation
				 * @param The regExp string
				 */
				std::string getRegex() const {
					return m_regexValue;
				};
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
				virtual void markToUpdateTextPosition();
				/**
				 * @brief update the display position start  == > depending of the position of the Cursor and the size of the Data inside
				 * @change m_displayStartPosition < ==  updated
				 */
				virtual void updateTextPosition();
				/**
				 * @brief change the cursor position with the curent position requested on the display
				 * @param[in] _pos Absolute position of the event
				 * @note The display is automaticly requested when change apear.
				 */
				virtual void updateCursorPosition(const vec2& _pos, bool _Selection=false);
			public:
				/**
				 * @brief Copy the selected data on the specify clipboard
				 * @param[in] _clipboardID Selected clipboard
				 */
				virtual void copySelectionToClipBoard(enum gale::context::clipBoard::clipboardListe _clipboardID);
				/**
				 * @brief remove the selected area
				 * @note This request a regeneration of the display
				 */
				virtual void removeSelected();
			private:
				ewol::parameter::Value<std::string> m_textWhenNothing; //!< Text to display when nothing in in the entry (decorated text...)
			public:
				/**
				 * @brief set The text displayed when nothing is in the entry.
				 * @param _text Text to display when the entry box is empty (this text can be decorated).
				 */
				void setEmptyText(const std::string& _text) {
					m_textWhenNothing.set(_text);
				}
				/**
				 * @brief get The text displayed when nothing is in the entry.
				 * @return Text display when nothing
				 */
				const std::string& getEmptyText() const {
					return m_textWhenNothing;
				};
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool onEventEntry(const ewol::event::Entry& _event);
				virtual void onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID);
				virtual void calculateMinMaxSize();
			protected: // Derived function
				virtual void onDraw();
				virtual void onGetFocus();
				virtual void onLostFocus();
				virtual void changeStatusIn(int32_t _newStatusId);
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			private: // callback functions
				void onCallbackShortCut(const std::string& _value);
				void onCallbackEntryClean();
				void onCallbackCut();
				void onCallbackCopy();
				void onCallbackPaste();
				void onCallbackSelect(bool _all);
		};
	};
};

#endif
