/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_PARAMETER_H__
#define __EWOL_WIDGET_PARAMETER_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WSlider.h>
#include <ewol/widget/meta/ParameterList.h>

extern const char * const ewolEventParameterClose;

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Parameter : public widget::PopUp {
		public:
			Parameter(void);
			~Parameter(void);
			// Derived function
			virtual const char * const getObjectType(void) {
				return "EwolParameter";
			};
			// Derived function
			virtual void onReceiveMessage(const ewol::EMessage& _msg);
			// Derived function
			virtual void onObjectRemove(ewol::EObject * _removeObject);
			
			void setTitle(std::string _label);
			void menuAdd(std::string _label, std::string _image, ewol::Widget* _associateWidget);
			void menuAddGroup(std::string _label);
			void menuClear(void);
			void menuSeparator(void);
		private:
			int32_t                 m_currentIdList;
			widget::Label*          m_widgetTitle;
			widget::ParameterList*  m_paramList;
			widget::WSlider*        m_wSlider;
	};
	
};

#endif
