/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_STD_POP_UP_H__
#define __EWOL_STD_POP_UP_H__

#include <ewol/widget/PopUp.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Sizer.h>
namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 * @brief The std pop up widget is a siple message widget to notyfy user of some simple things, like:
	 *
	 * ~~~~~~~~~~~~~~~~~~~~~
	 * 	+---------------------------------+---+---+---+
	 * 	| Windows name...                 | _ | O | X |
	 * 	+---------------------------------+---+---+---+
	 * 	|                                             |
	 * 	|                                             |
	 * 	|                                             |
	 * 	|            +-------------------+            |
	 * 	|            | Erreur:           |            |
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
	 * ~~~~~~~~~~~~~~~~~~~~~
	 */
	class StdPopUp : public widget::PopUp {
		public:
			/**
			 * @brief std-pop-up constructor.
			 */
			StdPopUp(void);
			/**
			 * @brief std-pop-up destructor.
			 */
			~StdPopUp(void);
		protected:
			widget::Label* m_title; //!< Title Label widget
		public:
			/**
			 * @brief Set the title string.
			 * @param[in] _text Decorated text to diplay in title.
			 */
			void setTitle(const std::string& _text);
		protected:
			widget::Label* m_comment; //!< Comment label widget
		public:
			/**
			 * @brief Set the commentary string.
			 * @param[in] _text Decorated text to diplay in Comment.
			 */
			void setComment(const std::string& _text);
		protected:
			widget::Sizer* m_subBar; //!< subwidget bar containing all the button.
		public:
			/**
			 * @brief Add a buttom button.
			 * @param[in] _text Decorated text to diplay in button.
			 */
			widget::Button* addButton(const std::string& _text, bool _autoExit=false);
		public: // Derived function
			virtual const char * const getObjectType(void) {
				return "widget::StdPopUp";
			};
			virtual void onObjectRemove(ewol::EObject* _removeObject);
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
	};
	/**
	 * @}
	 */
};

#endif
