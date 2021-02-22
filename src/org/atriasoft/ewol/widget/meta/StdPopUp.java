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
		using StdPopUpShared = ememory::SharedPtr<ewol::widget::StdPopUp>;
		using StdPopUpWeak = ememory::WeakPtr<ewol::widget::StdPopUp>;
		/**
		 * @brief The std pop up widget is a siple message widget to notify user of some simple things, like:
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
				eproperty::Value<etk::String> propertyTitle; //!< Title of the pop-up
				eproperty::Value<etk::String> propertyComment; //!< comment in the pop-up (can be decorated text)
			protected:
				/**
				 * @brief std-pop-up constructor.
				 */
				StdPopUp();
				void init();
			public:
				DECLARE_WIDGET_FACTORY(StdPopUp, "StdPopUp");
				/**
				 * @brief std-pop-up destructor.
				 */
				virtual ~StdPopUp();
			protected:
				ewol::widget::LabelShared m_title; //!< Title Label widget
				/**
				 * @brief property callback when request a change of the title.
				 */
				void onChangePropertyTitle();
				ewol::widget::LabelShared m_comment; //!< Comment label widget
				/**
				 * @brief property callback when request a change of the Comment.
				 */
				void onChangePropertyComment();
			protected:
				ewol::widget::SizerShared m_subBar; //!< subwidget bar containing all the button.
			public:
				/**
				 * @brief Add a buttom button.
				 * @param[in] _text Decorated text to diplay in button.
				 */
				ewol::widget::ButtonShared addButton(const etk::String& _text, bool _autoExit=false);
			public:
				virtual void onCallBackButtonExit();
		};
	}
}

