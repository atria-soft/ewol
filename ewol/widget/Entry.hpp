/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <regex>
#include <ewol/debug.hpp>
#include <ewol/compositing/Text.hpp>
#include <ewol/compositing/Drawing.hpp>
#include <ewol/compositing/Shaper.hpp>
#include <ewol/widget/Widget.hpp>
#include <etk/Color.hpp>
#include <ewol/widget/Manager.hpp>
#include <esignal/Signal.hpp>

namespace ewol {
	namespace widget {
		class Entry;
		using EntryShared = ememory::SharedPtr<ewol::widget::Entry>;
		using EntryWeak = ememory::WeakPtr<ewol::widget::Entry>;
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
			public: // Event list
				esignal::Signal<> signalClick; //!< bang on click the entry box
				esignal::Signal<std::string> signalEnter; //!< Enter key is pressed
				esignal::Signal<std::string> signalModify; //!< data change
			public: // propertie list
				eproperty::Value<std::string> propertyShape;
				eproperty::Value<std::string> propertyValue; //!< string that must be displayed
				eproperty::Range<int32_t> propertyMaxCharacter; //!< number max of xharacter in the list
				eproperty::Value<std::string> propertyRegex; //!< regular expression value
				eproperty::Value<std::string> propertyTextWhenNothing; //!< Text to display when nothing in in the entry (decorated text...)
			private:
				ewol::compositing::Shaper m_shaper;
				int32_t m_colorIdTextFg; //!< color property of the text foreground
				int32_t m_colorIdTextBg; //!< color property of the text background
				int32_t m_colorIdCursor; //!< color property of the text cursor
				int32_t m_colorIdSelection; //!< color property of the text selection
				ewol::compositing::Text m_text; //!< text display m_text
			protected:
				/**
				 * @brief Contuctor
				 * @param[in] _newData The USting that might be set in the Entry box (no event generation!!)
				 */
				Entry();
				void init() override;
			public:
				DECLARE_WIDGET_FACTORY(Entry, "Entry");
				/**
				 * @brief Destuctor
				 */
				virtual ~Entry();
			protected:
				/**
				 * @brief internal check the value with RegExp checking
				 * @param[in] _newData The new string to display
				 */
				void setInternalValue(const std::string& _newData);
			private:
				std::regex m_regex; //!< regular expression to check content
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
			public:
				void onRegenerateDisplay() override;
				bool onEventInput(const ewol::event::Input& _event) override;
				bool onEventEntry(const ewol::event::Entry& _event) override;
				void onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID) override;
				void calculateMinMaxSize() override;
			protected:
				void onDraw() override;
				void onGetFocus() override;
				void onLostFocus() override;
				virtual void changeStatusIn(int32_t _newStatusId);
			protected:
				esignal::Connection m_PCH; //!< Periodic call handle to remove it when needed
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
			private: // callback functions
				void onCallbackShortCut(const std::string& _value);
				void onCallbackEntryClean();
				void onCallbackCut();
				void onCallbackCopy();
				void onCallbackPaste();
				void onCallbackSelect(bool _all);
			protected:
				virtual void onChangePropertyShaper();
				virtual void onChangePropertyValue();
				virtual void onChangePropertyMaxCharacter();
				virtual void onChangePropertyRegex();
				virtual void onChangePropertyTextWhenNothing();
		};
	};
};
