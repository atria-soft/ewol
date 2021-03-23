/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/PopUp.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Button.hpp>
#include <ewol/widget/Sizer.hpp>
namespace ewol {
	namespace widget {
		class StdPopUp;
		using StdPopUp = ememory::Ptr<ewol::widget::StdPopUp>;
		using StdPopUpWeak = ememory::WeakPtr<ewol::widget::StdPopUp>;
		/**
		 * The std pop up widget is a siple message widget to notify user of some simple things, like:
		 *
		 * [pre]
		 * 	+---------------------------------+---+---+---+
		 * 	| Windows name...                 | _ | O | X |
		 * 	+---------------------------------+---+---+---+
		 * 	|                                             |
		 * 	|                                             |
		 * 	|                                             |
		 * 	|            +-------------------+            |
		 * 	|            | Title:            |            |
		 * 	|            |                   |            |
		 * 	|            | Message to diplay |            |
		 * 	|            | to user           |            |
		 * 	|            |                   |            |
		 * 	|            |             Close |            |
		 * 	|            +-------------------+            |
		 * 	|                                             |
		 * 	|                                             |
		 * 	|                                             |
		 * 	+---------------------------------------------+
		 * [/pre]
		 */
		class StdPopUp : public ewol::widget::PopUp {
			public: // properties:
				eproperty::Value<String> propertyTitle; //!< Title of the pop-up
				eproperty::Value<String> propertyComment; //!< comment in the pop-up (can be decorated text)
			protected:
				/**
				 * std-pop-up ructor.
				 */
				StdPopUp();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(StdPopUp, "StdPopUp");
				/**
				 * std-pop-up destructor.
				 */
				 ~StdPopUp();
			protected:
				ewol::widget::Label this.title; //!< Title Label widget
				/**
				 * property callback when request a change of the title.
				 */
				void onChangePropertyTitle();
				ewol::widget::Label this.comment; //!< Comment label widget
				/**
				 * property callback when request a change of the Comment.
				 */
				void onChangePropertyComment();
			protected:
				ewol::widget::Sizer this.subBar; //!< subwidget bar containing all the button.
			public:
				/**
				 * Add a buttom button.
				 * @param _text Decorated text to diplay in button.
				 */
				ewol::widget::Button addButton( String _text, boolean _autoExit=false);
			public:
				 void onCallBackButtonExit();
		};
	}
}

