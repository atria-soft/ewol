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
#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WSlider.h>
#include <ewol/widget/meta/ParameterList.h>


namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Parameter : public ewol::widget::PopUp {
			public:
				// Event list of properties
				static const char * const eventClose;
			public:
				Parameter(void);
				~Parameter(void);
			public: // Derived function
				virtual void onReceiveMessage(const ewol::object::Message& _msg);
				virtual void onObjectRemove(ewol::Object * _removeObject);
			public:
				void setTitle(std::string _label);
				void menuAdd(std::string _label, std::string _image, ewol::Widget* _associateWidget);
				void menuAddGroup(std::string _label);
				void menuClear(void);
				void menuSeparator(void);
			private:
				int32_t                 m_currentIdList;
				ewol::widget::Label*          m_widgetTitle;
				ewol::widget::ParameterList*  m_paramList;
				ewol::widget::WSlider*        m_wSlider;
		};
	};
};

#endif
