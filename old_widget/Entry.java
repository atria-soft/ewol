/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/RegEx.hpp>
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
		using Entry = ememory::Ptr<ewol::widget::Entry>;
		using EntryWeak = ememory::WeakPtr<ewol::widget::Entry>;
		/**
		 * @ingroup ewolWidgetGroup
		 * Entry box display :
		 *
		 * ~~~~~~~~~~~~~~~~~~~~~~
		 * 	----------------------------------------------
		 * 	| Editable Text                              |
		 * 	----------------------------------------------
		 * ~~~~~~~~~~~~~~~~~~~~~~
		 */
		class Entry : public Widget {
			public: // Event list
				esignal::Signal<> signalClick; //!< bang on click the entry box
				esignal::Signal<String> signalEnter; //!< Enter key is pressed
				esignal::Signal<String> signalModify; //!< data change
			public: // propertie list
				eproperty::Value<bool> propertyPassword; //!< Disable display of the content of the entry
				eproperty::Value<etk::Uri> propertyShape;
				eproperty::Value<String> propertyValue; //!< string that must be displayed
				eproperty::Range<int> propertyMaxCharacter; //!< number max of xharacter in the list
				eproperty::Value<String> propertyRegex; //!< regular expression value
				eproperty::Value<String> propertyTextWhenNothing; //!< Text to display when nothing in in the entry (decorated text...)
			private:
				ewol::compositing::Shaper this.shaper;
				int this.colorIdTextFg; //!< color property of the text foreground
				int this.colorIdTextBg; //!< color property of the text background
				int this.colorIdCursor; //!< color property of the text cursor
				int this.colorIdSelection; //!< color property of the text selection
				ewol::compositing::Text this.text; //!< text display this.text
			protected:
				/**
				 * Contuctor
				 * @param _newData The USting that might be set in the Entry box (no event generation!!)
				 */
				Entry();
				void init() ;
			public:
				DECLARE_WIDGET_FACTORY(Entry, "Entry");
				/**
				 * Destuctor
				 */
				 ~Entry();
			protected:
				/**
				 * internal check the value with RegExp checking
				 * @param _newData The new string to display
				 */
				void setInternalValue( String _newData);
			private:
				etk::RegEx<String> this.regex; //!< regular expression to check content
			private:
				boolean this.needUpdateTextPos; //!< text position can have change
				int this.displayStartPosition; //!< ofset in pixel of the display of the UString
				boolean this.displayCursor; //!< Cursor must be display only when the widget has the focus
				int this.displayCursorPos; //!< Cursor position in number of Char
				int this.displayCursorPosSelection; //!< Selection position end (can be befor or after cursor and == this.displayCursorPos chan no selection availlable
			protected:
				/**
				 * informe the system thet the text change and the start position change
				 */
				 void markToUpdateTextPosition();
				/**
				 * update the display position start  == > depending of the position of the Cursor and the size of the Data inside
				 * @change this.displayStartPosition < ==  updated
				 */
				 void updateTextPosition();
				/**
				 * change the cursor position with the curent position requested on the display
				 * @param _pos Absolute position of the event
				 * @note The display is automaticly requested when change apear.
				 */
				 void updateCursorPosition( Vector2f _pos, boolean _Selection=false);
			public:
				/**
				 * Copy the selected data on the specify clipboard
				 * @param _clipboardID Selected clipboard
				 */
				 void copySelectionToClipBoard(enum gale::context::clipBoard::clipboardListe _clipboardID);
				/**
				 * remove the selected area
				 * @note This request a regeneration of the display
				 */
				 void removeSelected();
			public:
				void onRegenerateDisplay() ;
				boolean onEventInput( ewol::event::Input _event) ;
				boolean onEventEntry( ewol::event::Entry _event) ;
				void onEventClipboard(enum gale::context::clipBoard::clipboardListe _clipboardID) ;
				void calculateMinMaxSize() ;
			protected:
				void onDraw() ;
				void onGetFocus() ;
				void onLostFocus() ;
				 void changeStatusIn(int _newStatusId);
			protected:
				esignal::Connection this.PCH; //!< Periodic call handle to remove it when needed
				/**
				 * Periodic call to update grapgic display
				 * @param _event Time generic event
				 */
				void periodicCall( ewol::event::Time _event);
			private: // callback functions
				void onCallbackShortCut( String _value);
				void onCallbackEntryClean();
				void onCallbackCut();
				void onCallbackCopy();
				void onCallbackPaste();
				void onCallbackSelect(boolean _all);
			protected:
				 void onChangePropertyPassword();
				 void onChangePropertyShaper();
				 void onChangePropertyValue();
				 void onChangePropertyMaxCharacter();
				 void onChangePropertyRegex();
				 void onChangePropertyTextWhenNothing();
		};
	};
};
